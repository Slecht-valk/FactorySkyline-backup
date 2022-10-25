// ILikeBanas


#include "FSConveyorPoleOperator.h"
#include "Buildables/FGBuildablePole.h"
#include "Hologram/FGConveyorPoleHologram.h"
#include "FGColoredInstanceMeshProxy.h"


AFGHologram* UFSConveyorPoleOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();

	AFGHologram* Hologram = CreateHologram();
	if (!Hologram) return nullptr;
	AFGConveyorPoleHologram* ConveyorPoleHologram = Cast<AFGConveyorPoleHologram>(Hologram);
	if (!ConveyorPoleHologram) return Hologram;

	AFGBuildablePole* SourcePole = Cast<AFGBuildablePole>(Source);

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

	ConveyorPoleHologram->SetHologramLocationAndRotation(Hit);
	ConveyorPoleHologram->DoMultiStepPlacement(false);
	ConveyorPoleHologram->SetPoleHeight(SourcePole->mHeight);
	
	return ConveyorPoleHologram;
}

AFGBuildable* UFSConveyorPoleOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	AFGBuildablePole* SourcePole = Cast<AFGBuildablePole>(Source);
	AFGBuildablePole* BuildablePole = Cast<AFGBuildablePole>(Buildable);

	BuildablePole->SetPoleHeight(SourcePole->mHeight);

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	return Buildable;
}

FSBuildableType UFSConveyorPoleOperator::GetType() const
{
	return FSBuildableType::Building;
}
