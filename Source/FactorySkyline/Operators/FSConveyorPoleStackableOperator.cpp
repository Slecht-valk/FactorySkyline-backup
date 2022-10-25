// ILikeBanas


#include "FSConveyorPoleStackableOperator.h"


AFGHologram* UFSConveyorPoleStackableOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();
	return CreateHologram();
}

AFGBuildable* UFSConveyorPoleStackableOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	return Buildable;
}

FSBuildableType UFSConveyorPoleStackableOperator::GetType() const
{
	return FSBuildableType::Building;
}

void UFSConveyorPoleStackableOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	TArray<FOverlapResult> Result;
	FCollisionShape Shape;
	FTransform Transform = Buildable->GetTransform();
	FVector Loc = Transform.GetLocation();

	Shape.SetBox(FVector(10.0f, 120.0f, 50.0f));
	World->OverlapMultiByChannel(Result, FVector(Loc.X, Loc.Y, Loc.Z + 200.0f), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);

	for (FOverlapResult& Res : Result) {
		AFGBuildable* Buildable = Cast<AFGBuildable>(Res.GetActor());
		if (Buildable) List.Add(Buildable);
	}
}