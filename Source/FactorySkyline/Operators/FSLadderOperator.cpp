// ILikeBanas


#include "FSLadderOperator.h"
#include "Buildables/FGBuildableLadder.h"
#include "Hologram/FGLadderHologram.h"


AFGHologram* UFSLadderOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();

	AFGHologram* Hologram = CreateHologram();
	if (!Hologram) return nullptr;
	AFGLadderHologram* LadderHologram = Cast<AFGLadderHologram>(Hologram);
	if (!LadderHologram) return Hologram;

	AFGBuildableLadder* SourceLadder = Cast<AFGBuildableLadder>(Source);
	
	LadderHologram->mTargetSegmentHeight = SourceLadder->mNumSegments;
	for (int i = 1; i < SourceLadder->mNumSegments; i++) {
		LadderHologram->mInstancedMeshComponent->AddInstance(FTransform(FVector(0.f, 1.f, 0.f).ToOrientationQuat(), FVector(0.0f, 0.0f, LadderHologram->mMeshHeight * i)));
	}

	return LadderHologram;
}

AFGBuildable* UFSLadderOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildableLadder* SourceLadder = Cast<AFGBuildableLadder>(Source);
	AFGBuildableLadder* TargetLadder = Cast<AFGBuildableLadder>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());
	TargetLadder->SetNumSegments(SourceLadder->mNumSegments);

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	return Buildable;
}

void UFSLadderOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	AFGBuildableLadder* Ladder = Cast<AFGBuildableLadder>(Buildable);
	if (Ladder) {
		TArray<FOverlapResult> Result;
		FCollisionShape Shape;
		FTransform Transform = Ladder->GetTransform();
		FVector Loc = Transform.GetLocation();
		Loc.Z += Ladder->mMeshHeight * Ladder->mNumSegments / 2.0;
		Shape.SetBox(FVector(10.0f, 10.0f, Ladder->mMeshHeight * Ladder->mNumSegments / 2.0 + 50.0f));
		World->OverlapMultiByChannel(Result, Loc, Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);
				
		for (FOverlapResult& Res : Result) {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Res.GetActor());
			if (Buildable) List.Add(Buildable);
		}
	}
}

