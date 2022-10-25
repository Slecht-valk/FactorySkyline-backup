// ILikeBanas

#pragma once

#include <sys/timeb.h>
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FGBuildableSubsystem.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableService.generated.h"

typedef TWeakObjectPtr<AFGBuildable> BuildablePtr;

struct FSTime
{
	_timeb StartTime;
	inline FSTime() { Start(); }
	inline void Start() { _ftime_s(&StartTime); }
	inline void Start(int delta) { _ftime_s(&StartTime); StartTime.time += delta; }
	inline double GetTime() {
		_timeb Current;
		_ftime_s(&Current);
		return (Current.time - StartTime.time) + (Current.millitm - StartTime.millitm)/1000.0;
	}
	static inline __int64 Current() { _timeb Current; _ftime_s(&Current); return Current.time * 1000 + Current.millitm; }
};

struct Float3D
{
	Float3D() { Pos[0] = Pos[1] = Pos[2] = 0.0f; };
	Float3D(const FVector& Vector) { Pos[0] = Vector.X; Pos[1] = Vector.Y; Pos[2] = Vector.Z; };
	inline Float3D& operator=(const Float3D& Other) { Pos[0] = Other.Pos[0]; Pos[1] = Other.Pos[1]; Pos[2] = Other.Pos[2]; }
	float Pos[3];
};

struct Float2D
{
	float Pos[2];
};

class KDTreeNode
{
public:
	KDTreeNode() { Left = Right = 0; }

	float Split;
	int16 Dim;
	TArray<BuildablePtr> Buildables;
	int32 Left;
	int32 Right;
};

class KDTree
{
public:
	KDTree() { Seed = FSTime::Current(); }

	TArray<KDTreeNode> Tree;
	const int MaxDim = 3;
	
	const int MinElementCount = 10;

	void BuildTree(const TSet<BuildablePtr>& BuildableSet);
	AFGBuildable* QueryNearest(const FVector& Vector, float& Dist);

private:

	void Partition(TArray<BuildablePtr>& Buildables, TArray<Float3D>& Pos, int d, int k, int& Start, int& End);
	void BuildTree(TArray<BuildablePtr>& Buildables, TArray<Float3D>& Pos, int NodeIndex, int D, int Start, int End);
	void QueryNearest(const Float3D& Pos, float& DistSqr, AFGBuildable*& Buildable, int NodeIndex);

	unsigned int Seed;

	inline unsigned int Random()
	{
		return Seed = Seed * 23411987 + 2345611 + rand();
	}
};


struct Int2D
{
	Int2D() { Pos[0] = Pos[1] = 0; };
	Int2D(const FVector2D& Vector) { Pos[0] = Vector.X; Pos[1] = Vector.Y; };
	inline Int2D& operator=(const Int2D& Other) { Pos[0] = Other.Pos[0]; Pos[1] = Other.Pos[1]; return *this; }
	inline const bool operator==(const Int2D& Other) const { return Pos[0] == Other.Pos[0] && Pos[1] == Other.Pos[1]; }
	int Pos[2];
};

struct KDTreeDataType2D
{
	TWeakObjectPtr<AFGBuildable> Buildable;
	Int2D Loc;
};

struct KDTreeNode2D
{
	KDTreeNode2D() { Left = Right = 0; }

	int Split;
	int Dim;
	int IndexStart;
	int IndexEnd;
	int Left;
	int Right;
};

class KDTree2D
{
public:
	KDTree2D() {}

	TArray<KDTreeDataType2D> Data;
	TArray<KDTreeNode2D> Tree;

	int SizeX;
	int SizeY;
	bool Reset;

	const int MaxDim = 2;
	const int MaxElementCount = 8;

	void BuildTree(const TSet<TWeakObjectPtr<AFGBuildable>>& BuildableSet, APlayerController* Player);
	void QueryDelta(TArray<TWeakObjectPtr<AFGBuildable> >& PositiveParam, TArray<TWeakObjectPtr<AFGBuildable> >& NagetiveParam, const Int2D& StartOld, const Int2D& EndOld, const Int2D& StartNew, const Int2D& EndNew);

private:

	TArray<TWeakObjectPtr<AFGBuildable> >* Positive;
	TArray<TWeakObjectPtr<AFGBuildable> >* Nagetive;

	void BuildTree(int NodeIndex, int Start, int End, Int2D* Box);
	void QueryDelta(int NodeIndex, Int2D* Box);

};


/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSBuildableService : public UObject
{
	GENERATED_BODY()
public:

	UWorld* World = nullptr;
	AFGPlayerController* FGController = nullptr;
	//AFGCharacterPlayer* Player = nullptr;
	AFGBuildableSubsystem* BuildableSubsystem = nullptr;

	KDTree StaticTree;

	TSet<BuildablePtr>* Current;
	TSet<BuildablePtr>* Backup;
	TSet<BuildablePtr> BuildableSet1;
	TSet<BuildablePtr> BuildableSet2;
	TArray<BuildablePtr> BuildableCache;

	void Init();

	void Update();

	UFUNCTION()
	void onBuildableCreated(AFGBuildable* Buildable);
};

inline float Sqrf(const float& x)
{
	return x * x;
}