// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FactorySkyline/FSBuildableService.h"
#include "FactorySkyline/FSDesign.h"
#include "Operators/FSBuildableOperator.h"
#include "Equipment/FGBuildGun.h"
#include "FGBuildableSubsystem.h"

#include<chrono>
#include<thread>

#include "FSSelectService.generated.h"

class FSConnectSelect : public FNonAbandonableTask
{
	//friend class FAsyncTask<FSConnectSelect>;
	friend class FAutoDeleteAsyncTask<FSConnectSelect>;
public:
	FSConnectSelect() { IsInited = false; Event = nullptr; }

	void NativeInit(UObject* WorldContext, UFSOperatorFactory* OperatorFactory);
	void DoWork();
	void ShutDown();

	void StartTask();
	void TerminalTask();
	bool IsReady();

	void Search_Positive();
	void Search_Nagetive();
	bool BFS_Nagetive(TWeakObjectPtr<AFGBuildable> StartPtr, TArray<TWeakObjectPtr<AFGBuildable> >& Result);

	UWorld* World;
	UFSOperatorFactory* OperatorFactory = nullptr;

	FEvent* Event;

	bool IsInited;
	bool NeedRunning;
	bool Ready;
	bool ForceStop;
	bool GetResult;
	int TaskType;

	FSBuildableType SearchType;

	TWeakObjectPtr<AFGBuildable> StartNode;
	TSet< TWeakObjectPtr<AFGBuildable> > Select;
	TMap< TWeakObjectPtr<AFGBuildable>, int> Mark;

	TArray< TWeakObjectPtr<AFGBuildable> > Result;
	TArray<TWeakObjectPtr<AFGBuildable> > Stack;

	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FSConnectSelect, STATGROUP_ThreadPoolAsyncTasks); }

	//~FSConnectSelect() { if (Event) FGenericPlatformProcess::ReturnSynchEventToPool(Event); }
};

/**
 *
 */
UCLASS()
class FACTORYSKYLINE_API UFSConnectSelectService : public UObject
{
	GENERATED_BODY()
public:

	void Init();
	void RefreshRecipe();

	bool Ready();
	bool GetLastResult(TArray< TWeakObjectPtr<AFGBuildable> >*& Result);
	bool SubmitConnectSelectTask(UFSDesign* Design, AFGBuildable* Buildable, int SelectType);
	void TerminalCurrentTask();

	virtual void BeginDestroy();
	virtual void FinishDestroy();


	UWorld* World = nullptr;
	AFGPlayerController* FGController = nullptr;
	//AFGBuildGun* FGBuildGun = nullptr;
	//AFGBuildableSubsystem* BuildableSubsystem = nullptr;

	//UFSSelection* Select = nullptr;
	UFSBuildableService* BuildableService = nullptr;

	UPROPERTY()
		UFSOperatorFactory* OperatorFactory = nullptr;

	FSConnectSelect* ConnectSelect = nullptr;
	FAutoDeleteAsyncTask<FSConnectSelect>* ConnectSelectAsyncTask;
};

class FSRectSelect : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<FSRectSelect>;
public:
	FSRectSelect() { IsInited = false; Event = nullptr; }

	void NativeInit(UObject* WorldContext);
	void DoWork();
	void Load();
	void ShutDown();

	void StartTask();
	void TerminalTask();
	bool IsReady();

	void Build();
	void Query();
	KDTree2D Tree;

	UWorld* World;

	FEvent* Event;

	bool IsInited;
	bool NeedRunning;
	bool Ready;
	bool GetResult;
	int TaskType;

	Int2D Start;
	Int2D Last;
	Int2D End;
	bool LeftClick;

	UFSDesign* Design;
	APlayerController* FGController;
	TSet<TWeakObjectPtr<AFGBuildable>> BuildableSet;
	TArray< TWeakObjectPtr<AFGBuildable> > Positive;
	TArray< TWeakObjectPtr<AFGBuildable> > Nagetive;

	FORCEINLINE TStatId GetStatId() const { RETURN_QUICK_DECLARE_CYCLE_STAT(FSRectSelect, STATGROUP_ThreadPoolAsyncTasks); }

	~FSRectSelect() { if (Event) FGenericPlatformProcess::ReturnSynchEventToPool(Event); }
};

/**
 *
 */
UCLASS()
class FACTORYSKYLINE_API UFSRectSelectService : public UObject
{
	GENERATED_BODY()
public:

	void Init();

	bool Ready();
	bool GetLastResult(TArray<TWeakObjectPtr<AFGBuildable> >*& Positive, TArray<TWeakObjectPtr<AFGBuildable> >*& Nagetive);
	bool StartRectSelect(UFSDesign* Design, const FVector2D& Start, bool LeftClick);
	bool QueryRectSelect(int x, int y);
	void TerminalCurrentTask();

	virtual void BeginDestroy();
	virtual void FinishDestroy();

	UWorld* World = nullptr;
	AFGPlayerController* FGController = nullptr;
	//AFGBuildGun* FGBuildGun = nullptr;

	UFSBuildableService* BuildableService = nullptr;

	FSRectSelect* RectSelect = nullptr;
	FAutoDeleteAsyncTask<FSRectSelect>* RectSelectAsyncTask;
};
