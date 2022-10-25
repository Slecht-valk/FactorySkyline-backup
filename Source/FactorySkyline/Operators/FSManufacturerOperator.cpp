// ILikeBanas


#include "FSManufacturerOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableManufacturer.h"
#include "Hologram/FGFactoryHologram.h"
#include "FGFactoryLegsComponent.h"

/*
AFGBuildable* UFSManufacturerOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	Buildable->SetBuildingID(Source->GetBuildingID());

	Buildable->FinishSpawning(Transform);

	UFGFactoryLegsComponent* Legs = Buildable->FindComponentByClass<UFGFactoryLegsComponent>();
	TArray< FFeetOffset > feetOffset = Legs->TraceFeetOffsets(Transform, Buildable);
	Legs->SetFeetOffsets(feetOffset);
	Legs->RecreateLegs();

	return Buildable;
}*/


void UFSManufacturerOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	Super::ApplySettingsTo(Buildable);

	AFGBuildableManufacturer* SourceManufacturer = Cast<AFGBuildableManufacturer>(Source);
	AFGBuildableManufacturer* TargetManufacturer = Cast<AFGBuildableManufacturer>(Buildable);

	TargetManufacturer->SetRecipe(SourceManufacturer->GetCurrentRecipe());
}
