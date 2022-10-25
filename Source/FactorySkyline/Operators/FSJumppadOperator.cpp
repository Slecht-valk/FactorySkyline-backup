// ILikeBanas


#include "FSJumppadOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableJumppad.h"
#include "Hologram/FGJumpPadLauncherHologram.h"
#include "FGFactoryLegsComponent.h"
#include "FGPipeConnectionComponent.h"


AFGHologram* UFSJumppadOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();

	AFGJumpPadLauncherHologram* Hologram = Cast<AFGJumpPadLauncherHologram>(CreateHologram());
	AFGBuildableJumppad* SourceJumppad = Cast<AFGBuildableJumppad>(Source);
	/*
	Hologram->DoMultiStepPlacement(false);
	Hologram->mLaunchAngle = SourceJumppad->mLaunchAngle;
	Hologram->ScrollRotate(10, 1);
	Hologram->ScrollRotate(-10, 1);
	//Hologram->OnLaunchAngleAdjusted();
	*/
	return Hologram;
}

AFGBuildable* UFSJumppadOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildableJumppad* SourceJumppad = Cast<AFGBuildableJumppad>(Source);
	AFGBuildableJumppad* TargetJumppad = Cast<AFGBuildableJumppad>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());
	TargetJumppad->SetLaunchAngle(SourceJumppad->mLaunchAngle);
	//SML::Logging::info(SourceJumppad->mLaunchAngle);

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	UFGFactoryLegsComponent* Legs = Buildable->FindComponentByClass<UFGFactoryLegsComponent>();
	if (Legs) {
		TArray< FFeetOffset > feetOffset = Legs->TraceFeetOffsets(Transform, Buildable);
		Legs->SetFeetOffsets(feetOffset);
		Legs->RecreateLegs();
	}

	return Buildable;
}

