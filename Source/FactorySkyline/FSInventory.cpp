// ILikeBanas


#include "FSInventory.h"
#include "FSkyline.h"
#include "FGRecipe.h"
#include "Resources/FGItemDescriptor.h"


void FSInventory::Init(UFSSplineHologramFactory* SplineHologramFactoryParam)
{
	this->SplineHologramFactory = SplineHologramFactoryParam;
}

void FSInventory::AddResource(FSInventory* Inventory, int Multiplier)
{
	if (!Inventory) return;
	for (TPair<TSubclassOf<UFGItemDescriptor>, int>& Pair : Inventory->Storage)
		AddResource(Pair.Key, Pair.Value * Multiplier);
}

void FSInventory::AddResource(UFSDesign* Design, int Multiplier)
{
	if (!Design) return;
	for (TWeakObjectPtr<AFGBuildable> BuildablePtr : Design->BuildableSet) {
		AFGBuildable* Buildable = BuildablePtr.Get();
		if (Buildable) this->AddResource(Buildable, Multiplier);
	}
}

void FSInventory::AddResource(UFGInventoryComponent* InventoryComponent, int Multiplier)
{
	if (!InventoryComponent) return;
	FInventoryStack out_stack;
	for (int i = 0; i < InventoryComponent->GetSizeLinear(); i++) {
		if (InventoryComponent->GetStackFromIndex(i, out_stack)) 
			this->AddResource(out_stack, Multiplier);
	}
}

void FSInventory::AddResource(AFGBuildable* Buildable, int Multiplier)
{
	if (!Buildable) return;
	TSubclassOf< class UFGRecipe > Recipe = SplineHologramFactory->GetRecipeFromClass(Buildable->GetClass());
	if (!Recipe) Recipe = Buildable->GetBuiltWithRecipe();
	if (!Recipe) return;

	//TODO: Figure out why GetDismantleRefundReturnsMultiplier() gives millions
	//this->AddResource(UFGRecipe::GetIngredients(Recipe), Buildable->GetDismantleRefundReturnsMultiplier() * Multiplier);
	this->AddResource(UFGRecipe::GetIngredients(Recipe), 1 * Multiplier);
}

void FSInventory::AddResourceCheckRecipe(AFGBuildable* Buildable, int Multiplier)
{
	if (!Buildable) return;
	TSubclassOf< class UFGRecipe > Recipe = Buildable->GetBuiltWithRecipe();
	if (!Recipe) return;

	//TODO: Figure out why GetDismantleRefundReturnsMultiplier() gives millions
	//this->AddResource(UFGRecipe::GetIngredients(Recipe), Buildable->GetDismantleRefundReturnsMultiplier() * Multiplier);
	this->AddResource(UFGRecipe::GetIngredients(Recipe), 1 * Multiplier);
}

void FSInventory::AddResource(const TArray<FItemAmount>& Items, int Multiplier)
{
	for (const FItemAmount& Item : Items) AddResource(Item, Multiplier);
}

void FSInventory::AddResource(const FItemAmount& Item, int Multiplier)
{
	//SML::Logging::info(Item.Amount);
	//SML::Logging::info(*Item.ItemClass->GetName());
	AddResource(Item.ItemClass, Item.Amount * Multiplier);
}

void FSInventory::AddResource(const TArray<FInventoryStack>& InventoryStack, int Multiplier)
{
	for (const FInventoryStack& Inventory : InventoryStack) AddResource(Inventory, Multiplier);
}

void FSInventory::AddResource(const FInventoryStack& Inventory, int Multiplier)
{
	//SML::Logging::info(Inventory.NumItems);
	//SML::Logging::info(*Inventory.Item.ItemClass->GetName());
	AddResource(Inventory.Item.ItemClass, Inventory.NumItems * Multiplier);
}

void FSInventory::AddResource(TSubclassOf<UFGItemDescriptor> ItemClass, int Multiplier)
{
	if (Multiplier) {
		FScopeLock ScopeLock(&Mutex);

		int* Value = Storage.Find(ItemClass);
		if (!Value) Value = &Storage.Add(ItemClass, Multiplier);
		else *Value += Multiplier;

		//SML::Logging::info(*ItemClass->GetName(), TEXT(" "), *Value);
		if (*Value == 0) Storage.Remove(ItemClass);
		Dirty = true;
	}
}

void FSInventory::Empty()
{
	FScopeLock ScopeLock(&Mutex);
	Storage.Empty();
	this->Dirty = true;
}

bool FSInventory::Valid(TMap<TSubclassOf<UFGItemDescriptor>, int>& Minus)
{
	FScopeLock ScopeLock(&Mutex);
	bool Res = true;
	Minus.Empty();
	for (TPair<TSubclassOf<UFGItemDescriptor>, int>& Pair : Storage)
		if (Pair.Value < 0) {
			Minus.Add(Pair.Key, -Pair.Value);
			Res = false;
		}
	return Res;
}

void FSInventory::GetStorageIfDirty(TMap<TSubclassOf<UFGItemDescriptor>, int>& StorageParam, bool& DirtyParam)
{
	FScopeLock ScopeLock(&Mutex);
	DirtyParam = this->Dirty;
	if (DirtyParam) StorageParam = this->Storage;
	this->Dirty = false;
}

int ConsumeInventory(UFGInventoryComponent* PlayerInventory, TSubclassOf<UFGItemDescriptor> ItemClass, int Count)
{
	if (!PlayerInventory) return Count;
	int left = PlayerInventory->GetNumItems(ItemClass);
	if (left >= Count) {
		PlayerInventory->Remove(ItemClass, Count);
		return 0;
	}
	if (left) PlayerInventory->Remove(ItemClass, left);
	return Count - left;
}

bool FSInventory::Consume(UFGInventoryComponent* PlayerInventory, const FSInventory* Cost)
{
	FScopeLock ScopeLock(&Mutex);
	bool Res = true;
	for (const TPair<TSubclassOf<UFGItemDescriptor>, int>& Pair : Cost->Storage) if (Pair.Value > 0) {
		int* value = Storage.Find(Pair.Key);
		int count = (value == nullptr ? 0 : *value);
		if (count <= Pair.Value) {
			if (value) Storage.Remove(Pair.Key);
			if (count < Pair.Value) {
				int left = ConsumeInventory(PlayerInventory, Pair.Key, Pair.Value - count);
				if (left) Storage.Add(Pair.Key, -left);
				Res &= (left == 0);
			}
		}
		else {
			*value -= Pair.Value;
		}
	}
	this->Dirty = true;
	return Res;
}

void FSInventory::TakeOut(UFGInventoryComponent* PlayerInventory, TSubclassOf<UFGItemDescriptor> ItemClass)
{
	FScopeLock ScopeLock(&Mutex);
	int *value = Storage.Find(ItemClass);
	if (value) {
		int count = UFGItemDescriptor::GetStackSize(ItemClass);
		if (count > *value) count = *value;

		FInventoryStack Stack(count, ItemClass);
		int TakeCount = PlayerInventory->AddStack(Stack, true);

		if (*value == TakeCount) Storage.Remove(ItemClass);
		else *value -= TakeCount;
		this->Dirty = true;
	}
}