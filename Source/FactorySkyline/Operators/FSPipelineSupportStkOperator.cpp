// ILikeBanas


#include "FSPipelineSupportStkOperator.h"
#include "FGBuildablePipelineSupport.h"


void UFSPipelineSupportStkOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
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