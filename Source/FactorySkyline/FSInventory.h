// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Core/Public/HAL/ThreadingBase.h"
#include "FSDesign.h"
#include "Operators/FSBuildableOperator.h"
#include "Buildables/FGBuildable.h"
#include "FGInventoryComponent.h"
#include "FSInventory.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FACTORYSKYLINE_API FSInventory
{
	GENERATED_BODY()
public:

	FSInventory() {}

	FSInventory(const FSInventory& Other) { SplineHologramFactory = Other.SplineHologramFactory; Storage = Other.Storage; }

	FSInventory& operator =(const FSInventory& Other) { SplineHologramFactory = Other.SplineHologramFactory; Storage = Other.Storage; Dirty = true; return *this; }

	void Init(UFSSplineHologramFactory* SplineHologramFactoryParam);
	void AddResource(FSInventory* Inventory, int Multiplier = 1);
	void AddResource(UFSDesign* Design, int Multiplier = 1);
	void AddResource(UFGInventoryComponent* InventoryComponent, int Multiplier = 1);
	void AddResource(AFGBuildable* Buildable, int Multiplier = 1);
	void AddResource(const TArray<FItemAmount>& Items, int Multiplier = 1);
	void AddResource(const FItemAmount& Item, int Multiplier = 1);
	void AddResource(const TArray<FInventoryStack>& InventoryStack, int Multiplier = 1);
	void AddResource(const FInventoryStack& Inventory, int Multiplier = 1);
	void AddResource(TSubclassOf<UFGItemDescriptor> ItemClass, int Multiplier = 1);
	void AddResourceCheckRecipe(AFGBuildable* Buildable, int Multiplier = 1);
	void Empty();

	bool Valid(TMap<TSubclassOf<UFGItemDescriptor>, int>& Minus);
	void GetStorageIfDirty(TMap<TSubclassOf<UFGItemDescriptor>, int>& StorageParam, bool& DirtyParam);
	bool Consume(UFGInventoryComponent* PlayerInventory, const FSInventory* Cost);
	void TakeOut(UFGInventoryComponent* PlayerInventory, TSubclassOf<UFGItemDescriptor> ItemClass);

	UFSSplineHologramFactory* SplineHologramFactory = nullptr;
	FCriticalSection Mutex;

	bool Dirty = true;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UFGItemDescriptor>, int> Storage;

	FORCEINLINE ~FSInventory() = default;
};
