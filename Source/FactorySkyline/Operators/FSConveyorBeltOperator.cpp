// ILikeBanas


#include "FSConveyorBeltOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableConveyorBelt.h"
//#include "FGInstancedSplineMesh.h"
#include "FGInstancedSplineMeshComponent.h"
#include "FactorySkyline/FSkyline.h"


AFGHologram* UFSConveyorBeltOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();
	AFGHologram* Hologram = CreateHologram();
	if (!Hologram) return nullptr;
	AFGConveyorBeltHologram* ConveyorBeltHologram = Cast<AFGConveyorBeltHologram>(Hologram);
	if (!ConveyorBeltHologram) return Hologram;

	AFGBuildableConveyorBelt* SourceBelt = Cast<AFGBuildableConveyorBelt>(Source);

	FHitResult Hit;
	Hit.Actor = nullptr;
	Hit.Time = 0.006946;
	Hit.Location = FVector(-11720.067f, 248538.719f, -10141.936f);
	Hit.ImpactPoint = FVector(-11720.066f, 248538.719f, -10141.936f);
	Hit.Normal = FVector(1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-11025.803f, 248538.188f, -10162.381f);
	Hit.TraceEnd = FVector(-110982.445f, 248615.406f, -12781.198f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	Hologram->SetHologramLocationAndRotation(Hit);
	Hologram->SetPlacementMaterialState(EHologramMaterialState::HMS_OK);

	UFGInstancedSplineMeshComponent* SourceComponent = Cast<UFGInstancedSplineMeshComponent>(SourceBelt->GetComponentByClass(UFGInstancedSplineMeshComponent::StaticClass()));
	USplineMeshComponent* SplineMeshComponent = nullptr;

	TSet<UActorComponent*> Set = Hologram->GetComponents();
	for (UActorComponent* Component : Set) {
		Log("%s", *Component->GetName());
		auto c = Cast<USplineMeshComponent>(Component);
		if(c) {
			SplineMeshComponent = Cast<USplineMeshComponent>(Component);
			break;
		}
	}

	bool NeedNew = false;
	for (FInstancedSplineInstanceData& Data : SourceComponent->PerInstanceSplineData) {
		if (NeedNew) {
			USplineMeshComponent* Component = NewObject<USplineMeshComponent>(Hologram);
			Component->SetStaticMesh(SplineMeshComponent->GetStaticMesh());
			Component->BodyInstance = SplineMeshComponent->BodyInstance;
			Component->SetForwardAxis(SplineMeshComponent->ForwardAxis);
			Component->SetMobility(SplineMeshComponent->Mobility);
			for (int i = 0; i < SplineMeshComponent->GetNumMaterials(); i++) {
				Component->SetMaterial(i, SplineMeshComponent->GetMaterial(i));
			}
			Component->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
			Component->AttachTo(Hologram->GetRootComponent());
			Component->RegisterComponent();
		}
		else {
			SplineMeshComponent->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
		}
		NeedNew = true;
	}
	return Hologram;
}

AFGBuildable* UFSConveyorBeltOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	FVector RelativeVector = TransformOperator.SourceTransform.InverseTransformPositionNoScale(Source->GetTransform().GetLocation());
	FQuat RelativeRotation = TransformOperator.SourceTransform.InverseTransformRotation(Source->GetTransform().GetRotation());
	FQuat Rotation = TransformOperator.TargetTransform.TransformRotation(RelativeRotation);

	FTransform Transform = FTransform(FRotator::ZeroRotator, TransformOperator.TargetTransform.TransformPositionNoScale(RelativeVector), Source->GetTransform().GetScale3D());

	AFGBuildableConveyorBelt* SourceConveyorBelt = Cast<AFGBuildableConveyorBelt>(Source);

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildableConveyorBelt* TargetConveyorBelt = Cast<AFGBuildableConveyorBelt>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//Buildable->SetBuildingID(Source->GetBuildingID());

	//TArray< FSplinePointData >* SourceData = &SourceConveyorBelt->mSplineData;
	TArray< FSplinePointData >* SourceData = FSkyline->AdaptiveUtil->GetConveyorBeltSplineData(SourceConveyorBelt);
	//TArray< FSplinePointData >* SourceData = &TargetConveyorBelt->mSplineData;
	TArray< FSplinePointData >* TargetData = FSkyline->AdaptiveUtil->GetConveyorBeltSplineData(TargetConveyorBelt);

	for (const FSplinePointData& PointData : *SourceData) {
		FSplinePointData NewPointData;
		NewPointData.Location = Rotation.RotateVector(PointData.Location);
		NewPointData.ArriveTangent = Rotation.RotateVector(PointData.ArriveTangent);
		NewPointData.LeaveTangent = Rotation.RotateVector(PointData.LeaveTangent);
		TargetData->Add(NewPointData);
	}

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	this->BuildableSubsystem->RemoveConveyorFromBucket(TargetConveyorBelt);

	return Buildable;
}


