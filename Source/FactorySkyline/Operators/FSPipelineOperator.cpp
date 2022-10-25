// ILikeBanas


#include "FSPipelineOperator.h"
#include "Buildables/FGBuildablePipeline.h"
#include "Buildables/FGBuildablePipeHyper.h"
#include "Hologram/FGPipelineHologram.h"
//#include "FGInstancedSplineMesh.h"
#include "FGInstancedSplineMeshComponent.h"
#include "FGPipeConnectionComponent.h"


AFGHologram* UFSPipelineOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();

	AFGHologram* Hologram = CreateHologram();
	if (!Hologram) return nullptr;
	AFGPipelineHologram* PipelineHologram = Cast<AFGPipelineHologram>(Hologram);
	if (!PipelineHologram) return Hologram;

	AFGBuildablePipeBase* SourcePipe = Cast<AFGBuildablePipeBase>(Source);
	//SML::Logging::info(*Hologram->GetFullName());

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

	PipelineHologram->SetHologramLocationAndRotation(Hit);
	PipelineHologram->SetPlacementMaterialState(EHologramMaterialState::HMS_OK);

	UFGInstancedSplineMeshComponent* SourceComponent = Cast<UFGInstancedSplineMeshComponent>(SourcePipe->GetComponentByClass(UFGInstancedSplineMeshComponent::StaticClass()));
	USplineMeshComponent* SplineMeshComponent = nullptr;

	TSet<UActorComponent*> Set = PipelineHologram->GetComponents();
	for (UActorComponent* Component : Set) {
		auto c = Cast<USplineMeshComponent>(Component);
		if(c) {
			SplineMeshComponent = Cast<USplineMeshComponent>(Component);
			break;
		}
	}

	bool NeedNew = false;
	for (FInstancedSplineInstanceData& Data : SourceComponent->PerInstanceSplineData) {
		if (NeedNew) {
			USplineMeshComponent* Component = NewObject<USplineMeshComponent>(PipelineHologram);
			Component->SetStaticMesh(SplineMeshComponent->GetStaticMesh());
			Component->BodyInstance = SplineMeshComponent->BodyInstance;
			Component->SetForwardAxis(SplineMeshComponent->ForwardAxis);
			Component->SetMobility(SplineMeshComponent->Mobility);
			for (int i = 0; i < SplineMeshComponent->GetNumMaterials(); i++) {
				Component->SetMaterial(i, SplineMeshComponent->GetMaterial(i));
			}
			Component->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
			Component->AttachTo(PipelineHologram->GetRootComponent());
			Component->RegisterComponent();
		} else {
			SplineMeshComponent->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
		}
		NeedNew = true;
	}
	return PipelineHologram;
}

AFGBuildable* UFSPipelineOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FVector RelativeVector = TransformOperator.SourceTransform.InverseTransformPositionNoScale(Source->GetTransform().GetLocation());
	FQuat RelativeRotation = TransformOperator.SourceTransform.InverseTransformRotation(Source->GetTransform().GetRotation());
	FQuat Rotation = TransformOperator.TargetTransform.TransformRotation(RelativeRotation);

	FTransform Transform = FTransform(FRotator::ZeroRotator, TransformOperator.TargetTransform.TransformPositionNoScale(RelativeVector), Source->GetTransform().GetScale3D());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);
	AFGBuildablePipeBase* SourcePipeline = Cast<AFGBuildablePipeBase>(Source);
	AFGBuildablePipeBase* TargetPipeline = Cast<AFGBuildablePipeBase>(Buildable);

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	TargetPipeline->mSplineData.Empty();
	for (const FSplinePointData& PointData : SourcePipeline->mSplineData) {
		FSplinePointData NewPointData;
		NewPointData.Location = Rotation.RotateVector(PointData.Location);
		NewPointData.ArriveTangent = Rotation.RotateVector(PointData.ArriveTangent);
		NewPointData.LeaveTangent = Rotation.RotateVector(PointData.LeaveTangent);
		TargetPipeline->mSplineData.Add(NewPointData);
	}

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	return Buildable;
}

void UFSPipelineOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
	UFGPipeConnectionComponentBase* SourceComponent = Cast<UFGPipeConnectionComponentBase>(SFC);
	UFGPipeConnectionComponentBase* TargetComponent = Cast<UFGPipeConnectionComponentBase>(TFC);

	if (!SourceComponent || !TargetComponent) return;

	if (!TargetComponent->IsConnected() && (SourceComponent->IsConnected() || Force)) {
		UFGPipeConnectionComponentBase* Connection = nullptr;
		if (SourceComponent->IsConnected()) {
			Connection = this->ConnectionMapping<UFGPipeConnectionComponentBase>(SourceComponent->GetConnection());
			if (!Connection) {
				FTransform Transform = TransformOperator.Transform(SourceComponent->GetConnection()->GetComponentTransform());
				Connection = UFGPipeConnectionComponentBase::FindCompatibleOverlappingConnection(TargetComponent, Transform.GetLocation(), 50.0f, TargetComponent);
			}
		}
		if (!Connection && Force) {
			Connection = UFGPipeConnectionComponentBase::FindCompatibleOverlappingConnection(TargetComponent, TargetComponent->GetComponentTransform().GetLocation(), 50.0f, TargetComponent);
		}
		if (Connection && Connection != TargetComponent && !Connection->IsConnected() && TargetComponent->CanConnectTo(Connection))
			TargetComponent->SetConnection(Connection);
	}
}

void UFSPipelineOperator::ApplyConnection(AFGBuildable* Buildable)
{
	AFGBuildablePipeBase* Source = Cast<AFGBuildablePipeBase>(this->Source);
	AFGBuildablePipeBase* Target = Cast<AFGBuildablePipeBase>(Buildable);

	UFGPipeConnectionComponentBase* SourceInput = Source->GetConnection0();
	UFGPipeConnectionComponentBase* SourceOutput = Source->GetConnection1();
	UFGPipeConnectionComponentBase* TargetInput = Target->GetConnection0();
	UFGPipeConnectionComponentBase* TargetOutput = Target->GetConnection1();

	if (!TargetInput->IsConnected() && SourceInput->IsConnected()) {
		UFGPipeConnectionComponentBase* Connection = this->ConnectionMapping<UFGPipeConnectionComponentBase>(SourceInput->GetConnection());
		if (!Connection) {
			Connection = UFGPipeConnectionComponentBase::FindCompatibleOverlappingConnection(TargetInput, TargetInput->GetComponentTransform().GetLocation(), 50.0f, TargetInput);
		}
		if (Connection && Connection != TargetInput && !Connection->IsConnected() && TargetInput->CanConnectTo(Connection))
			TargetInput->SetConnection(Connection);
	}

	if (!TargetOutput->IsConnected() && SourceOutput->IsConnected()) {
		UFGPipeConnectionComponentBase* Connection = this->ConnectionMapping<UFGPipeConnectionComponentBase>(SourceOutput->GetConnection());
		if (!Connection) {
			Connection = UFGPipeConnectionComponentBase::FindCompatibleOverlappingConnection(TargetOutput, TargetOutput->GetComponentTransform().GetLocation(), 50.0f, TargetOutput);
		}
		if (Connection && Connection != TargetOutput && !Connection->IsConnected() && TargetOutput->CanConnectTo(Connection))
			TargetOutput->SetConnection(Connection);
	}
}

FSBuildableType UFSPipelineOperator::GetType() const
{
	return FSBuildableType::Factory;
}

void UFSPipelineOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	for (UActorComponent* TargetConnection : TargetComponent)
		if (Cast<UFGPipeConnectionComponentBase>(TargetConnection)) {
			UFGPipeConnectionComponentBase* TFC = Cast<UFGPipeConnectionComponentBase>(TargetConnection);
			if (TFC->GetConnection()) {
				AFGBuildable* Connection = Cast<AFGBuildable>(TFC->GetConnection()->GetAttachmentRootActor());
				if (Connection) List.Add(Connection);
			}
		}
}