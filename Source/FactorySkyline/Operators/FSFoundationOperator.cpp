// ILikeBanas


#include "FSFoundationOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFoundation.h"


void UFSFoundationOperator::UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid)
{
	ShouldShow = Valid = false;
	if (!Hologram->IsValidHitResult(Hit)) return;
	
	if (Hologram->TrySnapToActor(Hit)) {
		ShouldShow = Valid = true;
		return;
	}

	Hologram->SetActorLocation(Hit.Location);
	Hologram->SetHologramLocationAndRotation(Hit);
	ShouldShow = Valid = true;
}

void UFSFoundationOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	AFGBuildableFoundation* Foundation = Cast<AFGBuildableFoundation>(Buildable);
	if (Foundation) {
		TArray<FOverlapResult> Result;
		FCollisionShape Shape;
		FTransform Transform = Foundation->GetTransform();
		FVector Loc = Transform.GetLocation();

		Shape.SetBox(FVector(Foundation->mDepth / 2.0 + 50.0f, Foundation->mWidth / 2.0 - 10.0f, Foundation->mHeight / 2.0 + Foundation->mElevation / 2.0 + 50.0f));
		World->OverlapMultiByChannel(Result, FVector(Loc.X, Loc.Y, Loc.Z + Foundation->mElevation / 2.0), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);

		Shape.SetBox(FVector(Foundation->mDepth / 2.0 - 10.0f, Foundation->mWidth / 2.0 + 50.0f, Foundation->mHeight / 2.0 + Foundation->mElevation / 2.0 + 50.0f));
		World->OverlapMultiByChannel(Result, FVector(Loc.X, Loc.Y, Loc.Z + Foundation->mElevation / 2.0), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);


		for (FOverlapResult& Res : Result) {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Res.GetActor());
			if (Buildable) List.Add(Buildable);
		}
	}
}

