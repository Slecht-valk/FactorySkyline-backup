// ILikeBanas


#include "FSPumpOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildablePipelinePump.h"

AFGBuildable* UFSPumpOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildablePipelinePump* SourcePump = Cast<AFGBuildablePipelinePump>(Source);
	AFGBuildablePipelinePump* TargetPump = Cast<AFGBuildablePipelinePump>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	TargetPump->mDefaultFlowLimit = SourcePump->mDefaultFlowLimit;
	TargetPump->SetUserFlowLimit(SourcePump->GetFlowLimit());

	TargetPump->mIndicatorFlowPct = SourcePump->mIndicatorFlowPct;
	TargetPump->mIndicatorPressurePct = SourcePump->mIndicatorPressurePct;

	return Buildable;
}

