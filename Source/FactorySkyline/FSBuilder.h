// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FSDesign.h"
#include "FSMenuItem.h"
#include "FSBuildableService.h"
#include "FSSyncWork.h"
#include "FSInventory.h"
#include "Operators/FSBuildableOperator.h"
#include "FGPlayerController.h"
#include "Equipment/FGBuildGun.h"
#include "Equipment/FGEquipment.h"
#include "FSBuilder.generated.h"

class UFSSyncBuild;

struct FSRepeat
{
	FTransform Source;
	FTransform Target;
	FTransform NextRelative;
	bool Ready = false;

	FTransform Next() {
		FVector Loc = Target.TransformPositionNoScale(NextRelative.GetLocation());
		FQuat Quat = Target.TransformRotation(NextRelative.GetRotation());
		return FTransform(Quat, Loc);
	}
};

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API AFSBuilder : public AActor
{
	GENERATED_BODY()

public:
	void Init();

	bool FindRecipeByName(const FString& Name, TSubclassOf< class UFGRecipe >& Result);

	bool CheckAnchor(UFSDesign* DesignParam);
	void Load(UFSDesign* DesignParam, bool FullPreview);
	void Unload();

	bool Test(const FHitResult& Hit);
	bool Build(FSRepeat* Repeat);
	bool CheckReady(float TimeLimit);
	void Update(const FHitResult& Hit);
	void Update(FSRepeat* Repeat);
	FTransform GetFixedTargetTransform(AFGHologram* HologramParam, bool& Valid);
	FTransform GetFixedSourceTransform();

	bool CheckCost();
	bool Consume();

	void ScrollUp();
	void ScrollDown();

	void onBuildFinish();

	bool IsBuilding = false;
	UFSDesign* Design = nullptr;

	UFGConnectionComponent* AnchorConnection;
	UFGFactoryConnectionComponent* AnchorInput;
	UFGFactoryConnectionComponent* AnchorOutput;

	bool LastShow;
	bool LastValid;
	FTransform Source;
	FTransform Target;
	FSInventory Cost;
	FSInventory* Inventory;

	UWorld* World = nullptr;
	AFGPlayerController* FGController = nullptr;

	UFSOperatorFactory* OperatorFactory = nullptr;
	UFSSplineHologramFactory* SplineHologramFactory = nullptr;
	UFSBuildableService* BuildableService = nullptr;
	UObject* FSCtrl = nullptr;
	UObject* SkylineUI = nullptr;

	UPROPERTY()
	UFSBuildableOperator* AnchorOperator;

	UPROPERTY()
	TArray< TSubclassOf< class UFGRecipe > > RecipesCache;

	UPROPERTY()
	AFGHologram* Hologram;

	UPROPERTY()
	TArray<AFGHologram*> HologramList;

	UPROPERTY()
	TArray<FTransform> RelativeTransform;

	UPROPERTY()
	UFSSyncBuild* SyncBuild;

	//UPROPERTY()
	//UFSInventory* Inventory;

	~AFSBuilder();
};

UCLASS()
class FACTORYSKYLINE_API UFSSyncBuild : public UFSSyncWork
{
	GENERATED_BODY()

public:

	virtual void Load();
	virtual void Unload();
	virtual void PreWork();
	virtual bool DoWork(float TimeLimitParam);
	void StepA();
	void StepB();
	void StepC();

	virtual int GetTotal();
	virtual int GetCurrent();

	TMap<AFGBuildable*, AFGBuildable*> BuildableMapping;
	TMap<UFGConnectionComponent*, UFGConnectionComponent*> InternalConnectionMapping;

	TArray<UFSBuildableOperator*> UFSBuildableOperatorList;
	TArray<AFGBuildablePassthrough*> passThroughBuildableList;
	TArray<AFGBuildablePassthrough*> passThroughNewList;
	//TArray<class AFGBuildablePassthrough*> passThroughNewList;

	TSet< TWeakObjectPtr<AFGBuildable> > BuildableSet;
	TWeakObjectPtr<AFGBuildable> Anchor;

	UFSDesign* NewDesign = nullptr;
	TWeakObjectPtr<UFSDesignMenu> CurrentDesignMenu;

	UPROPERTY()
	UFSDesignMenu* NewDesignMenu;

	FSTransformOperator FSTransform;
	UFSOperatorFactory* OperatorFactory;

	TArray<TWeakObjectPtr<AFGBuildable> > List;
	float TimeLimit;
	FSTime Time;
	int Step;
	int Index;
	int Total;
	int Current;
};
