// ILikeBanas


#include "FSBuildablePassThroughOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "Buildables/FGBuildablePassthrough.h"
#include "Hologram/FGConveyorLiftHologram.h"
#include "FactorySkyline/FSkyline.h"

void UFSBuildablePassThroughOperator::UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid)
{
	AFGConveyorLiftHologram* LiftHologram = Cast<AFGConveyorLiftHologram>(Hologram);

	ShouldShow = Valid = false;
	if (!Hologram->IsValidHitResult(Hit)) return;

	Hologram->SetHologramLocationAndRotation(Hit);

	AActor* Actor = Hit.GetActor();
	this->HitConnection = nullptr;

	if (!Actor) return;

	FVector HologramLocation = Hologram->GetTransform().GetLocation();

	for (UActorComponent* Connection : Actor->GetComponentsByClass(UFGFactoryConnectionComponent::StaticClass())) {
		UFGFactoryConnectionComponent* FactoryConnection = Cast<UFGFactoryConnectionComponent>(Connection);
		if (FactoryConnection) {
			if (HitConnection == nullptr) HitConnection = FactoryConnection;
			else {
				FVector HitLocation = HitConnection->GetComponentLocation();
				FVector FactoryLocation = FactoryConnection->GetComponentLocation();
				if ((FactoryLocation - HologramLocation).SizeSquared() < (HitLocation - HologramLocation).SizeSquared())
					HitConnection = FactoryConnection;
			}
		}
	}

	ShouldShow = Valid = true;
}
/*
AFGHologram* UFSBuildablePassThroughOperator::HologramCopy(FTransform& RelativeTransform)
{
	return SplineHologramFactory->CreateLiftHologram(Cast<AFGBuildableConveyorLift>(Source), RelativeTransform);
}
*/
AFGBuildable* UFSBuildablePassThroughOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	FTransform Transform = TransformOperator.Transform(Source->GetTransform());
	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildablePassthrough* SourceBuildablePassthrough = Cast<AFGBuildablePassthrough>(Source);
	AFGBuildablePassthrough* BuildablePassthrough = Cast<AFGBuildablePassthrough>(Buildable);

	float num = SourceBuildablePassthrough->mSnappedBuildingThickness;
	BuildablePassthrough->mSnappedBuildingThickness = num;

	//AFGBuildableConveyorLift* SourceConveyorLift = Cast<AFGBuildableConveyorLift>(Source);
	//AFGBuildableConveyorLift* TargetConveyorLift = Cast<AFGBuildableConveyorLift>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//Buildable->SetBuildingID(Source->GetBuildingID());
	/*
	TargetConveyorLift->mTopTransform = SourceConveyorLift->mTopTransform;
	TargetConveyorLift->mIsReversed = SourceConveyorLift->mIsReversed;
	*/
	//FSkyline->AdaptiveUtil->CopyConveyorLiftAttribute(SourceConveyorLift, TargetConveyorLift);

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	//this->BuildableSubsystem->RemoveConveyorFromBucket(TargetConveyorLift);

	return Buildable;
}
