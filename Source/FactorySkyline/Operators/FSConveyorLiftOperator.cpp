// ILikeBanas


#include "FSConveyorLiftOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "Hologram/FGConveyorLiftHologram.h"
#include "FactorySkyline/FSkyline.h"

void UFSConveyorLiftOperator::UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid)
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

AFGHologram* UFSConveyorLiftOperator::HologramCopy(FTransform& RelativeTransform)
{
	return SplineHologramFactory->CreateLiftHologram(Cast<AFGBuildableConveyorLift>(Source), RelativeTransform);
}

AFGBuildable* UFSConveyorLiftOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	FTransform Transform = TransformOperator.Transform(Source->GetTransform());
	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);

	AFGBuildableConveyorLift* SourceConveyorLift = Cast<AFGBuildableConveyorLift>(Source);
	AFGBuildableConveyorLift* TargetConveyorLift = Cast<AFGBuildableConveyorLift>(Buildable);

	TargetConveyorLift->mSnappedPassthroughs.Add(nullptr);
	TargetConveyorLift->mSnappedPassthroughs.Add(nullptr);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//Buildable->SetBuildingID(Source->GetBuildingID());
	/*
	TargetConveyorLift->mTopTransform = SourceConveyorLift->mTopTransform;
	TargetConveyorLift->mIsReversed = SourceConveyorLift->mIsReversed;
	*/
	//TSubclassOf< UFGRecipe > passThroughRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Foundations/Recipe_FoundationPassthrough_Lift.Recipe_FoundationPassthrough_Lift_C"));
	//TArray< class AFGBuildablePassthrough* > ThroughsSource = SourceConveyorLift->GetSnappedPassthroughs();
	TArray< class AFGBuildablePassthrough* > ThroughsSource = SourceConveyorLift->mSnappedPassthroughs;

	AFGBuildablePassthrough* BottomPassthrough = ThroughsSource[0];
	AFGBuildablePassthrough* TopPassthrough = ThroughsSource[1];
	if (BottomPassthrough) {
		//FSkyline->Select->Design->AddElement(Cast< AFGBuildable>(BottomPassthrough));
		//FSkyline->Select->Design->BuildableMark.Add(Cast< AFGBuildable>(BottomPassthrough), 1);
		//FSkyline->Select->EnableHightLight(Cast< AFGBuildable>(BottomPassthrough), FSkyline->Select->SelectMaterial);

		///FTransform  BottomPassthroughTransform = TransformOperator.Transform(BottomPassthrough->GetTransform());
		///AFGBuildable* BottomPassthroughBuildable = BuildableSubsystem->BeginSpawnBuildable(BottomPassthrough->GetClass(), BottomPassthroughTransform);

		///float num0 = BottomPassthrough->mSnappedBuildingThickness;
		///Cast<AFGBuildablePassthrough>(BottomPassthroughBuildable)->mSnappedBuildingThickness = num0;

		//TargetConveyorLift->mSnappedPassthroughs[0] = Cast<AFGBuildablePassthrough>(BottomPassthroughBuildable);
		//TargetConveyorLift->mSnappedPassthroughs[0] = BottomPassthrough;
	}
	if (TopPassthrough) {
		//FSkyline->Select->Design->AddElement(Cast< AFGBuildable>(TopPassthrough));
		//FSkyline->Select->Design->BuildableMark.Add(Cast< AFGBuildable>(TopPassthrough), 1);
		//FSkyline->Select->EnableHightLight(Cast< AFGBuildable>(TopPassthrough), FSkyline->Select->SelectMaterial);

		///FTransform  TopPassthroughTransform = TransformOperator.Transform(TopPassthrough->GetTransform());
		///AFGBuildable* TopPassthroughBuildable = BuildableSubsystem->BeginSpawnBuildable(TopPassthrough->GetClass(), TopPassthroughTransform);

		///float num1 = TopPassthrough->mSnappedBuildingThickness;
		///Cast<AFGBuildablePassthrough>(TopPassthroughBuildable)->mSnappedBuildingThickness = num1;

		//TargetConveyorLift->mSnappedPassthroughs[1] = Cast<AFGBuildablePassthrough>(TopPassthroughBuildable);
		//TargetConveyorLift->mSnappedPassthroughs[1] = TopPassthrough;
	}
	// this wouldn't have any pass throughs connected to it yet so no point checking
	//TArray< class AFGBuildablePassthrough* > ThroughsTarget = TargetConveyorLift->GetSnappedPassthroughs();
	//bool foundPassThrough = false;
	//if (ThroughsSource.Num() > 0) {
		//foundPassThrough = true;
	//}
	/*
	if (ThroughsTarget.Num() > 0) {
		foundPassThrough = true;
	}
	*/
	//if (foundPassThrough) {
	//}

	if (bottomPassThrough && topPassThrough) {
		//UFGConnectionComponent * Connection = Cast<UFGConnectionComponent>(Buildable->GetComponentByClass(UFGConnectionComponent::StaticClass()));
		/*
		bottomPassThrough->SetBottomSnappedConnection(Connection);
		TargetConveyorLift->mSnappedPassthroughs[1] = bottomPassThrough;
		topPassThrough->SetTopSnappedConnection(Connection);
		TargetConveyorLift->mSnappedPassthroughs[0] = topPassThrough;
		*/

		TargetConveyorLift->mSnappedPassthroughs[1] = bottomPassThrough;
		TargetConveyorLift->mSnappedPassthroughs[0] = topPassThrough;

		UFGConnectionComponent * Connection0 = Cast< AFGBuildableConveyorBase>(Buildable)->GetConnection0();
		UFGConnectionComponent * Connection1 = Cast< AFGBuildableConveyorBase>(Buildable)->GetConnection1();
		topPassThrough->SetBottomSnappedConnection(Connection1);
		bottomPassThrough->SetTopSnappedConnection(Connection0);
	}

	// for these connection types the initialization of the connect most always come before calling FinishedSpawning
	if (topPassThrough && !bottomPassThrough) {
		//UFGConnectionComponent * Connection = Cast<UFGConnectionComponent>(Buildable->GetComponentByClass(UFGConnectionComponent::StaticClass()));

		UFGConnectionComponent * Connection1 = Cast< AFGBuildableConveyorBase>(Buildable)->GetConnection1();
		topPassThrough->SetTopSnappedConnection(Connection1);
		TargetConveyorLift->mSnappedPassthroughs[0] = topPassThrough;
	}

	if (!topPassThrough && bottomPassThrough) {
		//UFGConnectionComponent * Connection = Cast<UFGConnectionComponent>(Buildable->GetComponentByClass(UFGConnectionComponent::StaticClass()));

		UFGConnectionComponent * Connection0 = Cast< AFGBuildableConveyorBase>(Buildable)->GetConnection0();
		bottomPassThrough->SetBottomSnappedConnection(Connection0);
		TargetConveyorLift->mSnappedPassthroughs[0] = bottomPassThrough;
	}

	// set this back to default state
	bottomPassThrough = topPassThrough = nullptr;

	FSkyline->AdaptiveUtil->CopyConveyorLiftAttribute(SourceConveyorLift, TargetConveyorLift);
	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);


	this->BuildableSubsystem->RemoveConveyorFromBucket(TargetConveyorLift);

	return Buildable;
}