// ILikeBanas


#include "FSWalkwayOperator.h"
#include "Buildables/FGBuildableFactoryBuilding.h"
#include "Buildables/FGBuildableWalkway.h"


void UFSWalkwayOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	AFGBuildableWalkway* Walkway = Cast<AFGBuildableWalkway>(Buildable);
	if (Walkway) {
		TArray<FOverlapResult> Result;
		FCollisionShape Shape;
		if (Walkway->mElevation > 10.0f) {
			FTransform Transform = Walkway->GetTransform();
			FVector Loc = Transform.GetLocation();
		
			Shape.SetBox(FVector(Walkway->mSize / 2.0, Walkway->mSize / 2.0 - 10.0f, Walkway->mElevation / 2.0));
			World->OverlapMultiByChannel(Result, FVector(Loc.X + 50.0f, Loc.Y, Loc.Z + Walkway->mElevation / 2.0 + 50.0f), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);
			World->OverlapMultiByChannel(Result, FVector(Loc.X - 50.0f, Loc.Y, Loc.Z + Walkway->mElevation / 2.0 - 50.0f), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);

			Shape.SetBox(FVector(Walkway->mSize / 2.0 - 10.0f, Walkway->mSize / 2.0 + 50.0f, Walkway->mElevation / 2.0 - 10.0f));
			World->OverlapMultiByChannel(Result, FVector(Loc.X, Loc.Y, Loc.Z + Walkway->mElevation / 2.0), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);
		}
		else {
			FTransform Transform = Walkway->GetTransform();

			Shape.SetBox(FVector(Walkway->mSize / 2.0 + 50.0f, Walkway->mSize / 2.0 - 10.0f, 50.0f));
			World->OverlapMultiByChannel(Result, Transform.GetLocation(), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);

			Shape.SetBox(FVector(Walkway->mSize / 2.0 - 10.0f, Walkway->mSize / 2.0 + 50.0f, 50.0f));
			World->OverlapMultiByChannel(Result, Transform.GetLocation(), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);
		}
		
		for (FOverlapResult& Res : Result) {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Res.GetActor());
			if (Buildable) List.Add(Buildable);
		}
	}
}

