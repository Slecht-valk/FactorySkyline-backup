// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildablePole.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "Buildables/FGBuildablePipeline.h"
#include "Buildables/FGBuildablePipeHyper.h"
#include "Hologram/FGHologram.h"
#include "Hologram/FGConveyorBeltHologram.h"
#include "Hologram/FGConveyorLiftHologram.h"
#include "Hologram/FGPipelineHologram.h"
#include "Equipment/FGBuildGun.h"
#include "FGCharacterPlayer.h"
#include "FGBuildableSubsystem.h"
#include "FactorySkyline/FSBuildableService.h"
#include "FGBuildablePipelineSupport.h"
#include "FSBuildableOperator.generated.h"


enum FSBuildableType
{
	Building,
	Factory,
	Power,
	Rail,
	Unknown
};

struct FSTransformOperator
{
public:

	FSTransformOperator() {};
	FSTransformOperator(const FTransform& Source, const FTransform& Target);

	FTransform SourceTransform;
	FTransform TargetTransform;

	FTransform Transform(const FTransform& Transform) const;
	FVector Transform(const FVector& Vector) const;
	FQuat Transform(const FQuat& Quat) const;
	FTransform InverseTransform(const FTransform& Transform) const;
};

UCLASS()
class FACTORYSKYLINE_API UFSSplineHologramFactory : public UObject
{
	GENERATED_BODY()
public:

	AFGBuildablePole* ConveyorPole;
	AFGBuildablePipelineSupport* PipelineSupport;
	AFGBuildablePipelineSupport* PipelineHyperSupport;

	AFGBuildableSubsystem* BuildableSubsystem = nullptr;
	//AFGBuildGun* FGBuildGun = nullptr;
	AActor* Builder = nullptr;
	UObject* Skyline;

	TArray< TSubclassOf<UFGRecipe> > CacheRecipes;
	TMap< TSubclassOf<AFGBuildable>, TSubclassOf<UFGRecipe> > RecipesMapping;

	void Init();

	void Load();
	void Unload();

	TSubclassOf<UFGRecipe> GetRecipeFromClass(TSubclassOf<AFGBuildable> BuildableClass);

	//AFGConveyorBeltHologram* CreateBeltHologram(AFGBuildableConveyorBelt* ConveyorBelt, FTransform& RelativeTransform);
	AFGConveyorLiftHologram* CreateLiftHologram(AFGBuildableConveyorLift* ConveyorBelt, FTransform& RelativeTransform);
	AFGPipelineHologram* CreatePipelineHologram(AFGBuildablePipeline* Pipeline, FTransform& RelativeTransform);
	AFGPipelineHologram* CreatePipelineHologram(AFGBuildablePipeHyper* Pipeline, FTransform& RelativeTransform);
};

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSBuildableOperator : public UObject
{
	GENERATED_BODY()
public:
	
	inline void LoadBuildable(AFGBuildable*& Buildable) { this->Source = Buildable; }

	virtual AFGHologram* CreateHologram();
	virtual void UpdateHologramState(const FHitResult &Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid);
	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
	virtual void UpdateInternelConnection(AFGBuildable* Buildable);
	virtual void ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplyConnection(AFGBuildable* Buildable, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplySettingsTo(AFGBuildable* Buildable);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> > & List) const;

	template<class T>
	inline T* ConnectionMapping(UFGConnectionComponent* Source);

	UFSBuildableOperator* SetUnknown();
	
	bool Unknown = false;

	//bool firstBuild = false;

	FSBuildableType TypeCache;

	AFGBuildable* Source;
	UWorld* World;
	UObject* Skyline;
	AActor* Builder;
	UFSSplineHologramFactory* SplineHologramFactory;
	AFGBuildableSubsystem* BuildableSubsystem;
	UFSBuildableService* BuildableService;

	TMap<AFGBuildable*, AFGBuildable*>* BuildableMapping;
	TMap<UFGConnectionComponent*, UFGConnectionComponent*>* InternalConnectionMapping;
	//TMap<AFGBuildableConveyorBase*, int32>* BucketIDMapping;
	//TMap<int32, TArray<AFGBuildableConveyorBase*> >* BucketMapping;

	UClass* sourceClass;
	FSTransformOperator transform;
	AFGBuildable* Buildable;

	AFGBuildablePassthrough* bottomPassThrough;
	AFGBuildablePassthrough* topPassThrough;
};

UCLASS()
class FACTORYSKYLINE_API UFSOperatorFactory : public UObject
{
	GENERATED_BODY()
public:

	UWorld* World = nullptr;
	UObject* Skyline = nullptr;
	AActor* Builder = nullptr;
	UFSSplineHologramFactory* SplineHologramFactory = nullptr;
	AFGBuildableSubsystem* BuildableSubsystem = nullptr;
	UFSBuildableService* BuildableService = nullptr;

	TMap<AFGBuildable*, AFGBuildable*>* BuildableMapping;
	TMap<UFGConnectionComponent*, UFGConnectionComponent*>* InternalConnectionMapping;
	//TMap<AFGBuildableConveyorBase*, int32>* BucketIDMapping;
	//TMap<int32, TArray<AFGBuildableConveyorBase*> >* BucketMapping;

	UPROPERTY()
	TMap<TSubclassOf<AFGBuildable>, UFSBuildableOperator*> Map;

	void Init();

	UFSBuildableOperator* AcquireOperator(AFGBuildable* Buildable);
	UFSBuildableOperator* CreateOperator(AFGBuildable* Buildable);
	UFSBuildableOperator* CreateEmptyOperator(UClass* Buildable);
	UFSBuildableOperator* EnsureCache(UClass* Buildable);
	void InitOperator(UFSBuildableOperator* Operator);

	inline FSBuildableType GetType(AFGBuildable*& Buildable) const
	{
		UFSBuildableOperator* const* Ptr = Map.Find(Buildable->GetClass());
		return Ptr ? (*Ptr)->TypeCache : FSBuildableType::Unknown;
	}

	inline void GetSelectConnectList(AFGBuildable*& Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const
	{
		UFSBuildableOperator* const* Ptr = Map.Find(Buildable->GetClass());
		if (Ptr && *Ptr) (*Ptr)->GetSelectConnectList(Buildable, List);
	}

	inline bool Check(TSubclassOf<AFGBuildable> Buildable, const FString& Name) const
	{ return Buildable->GetName().Equals(Name); }

	inline bool CheckContains(TSubclassOf<AFGBuildable> Buildable, const FString& Name) const
	{ return Buildable->GetName().Contains(Name); }
};

template<class T>
inline T* UFSBuildableOperator::ConnectionMapping(UFGConnectionComponent* Source)
{
	if (!Source) return nullptr;
	UFGConnectionComponent** Result = InternalConnectionMapping->Find(Source);
	return Result ? Cast<T>(*Result) : nullptr;
}
