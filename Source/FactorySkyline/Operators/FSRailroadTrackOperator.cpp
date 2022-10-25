// ILikeBanas


#include "FSRailroadTrackOperator.h"
#include "Hologram/FGRailroadTrackHologram.h"
#include "Buildables/FGBuildableRailroadTrack.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "FGInstancedSplineMeshComponent.h"
#include "FGRailroadSubsystem.h"


AFGHologram* UFSRailroadTrackOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();
	AFGRailroadTrackHologram* Hologram = Cast<AFGRailroadTrackHologram>(CreateHologram());
	if (!Hologram) return nullptr;

	AFGBuildableRailroadTrack* Track = Cast<AFGBuildableRailroadTrack>(Source);

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

	UFGInstancedSplineMeshComponent* SourceComponent = Cast<UFGInstancedSplineMeshComponent>(Track->GetComponentByClass(UFGInstancedSplineMeshComponent::StaticClass()));
	USplineMeshComponent* SplineMeshComponent = nullptr;

	TSet<UActorComponent*> Set = Hologram->GetComponents();
	for (UActorComponent* Component : Set) {
		if (Component->GetName().Equals(TEXT("SplineMeshComponent_0"))) {
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
			for (int i = 0; i < SplineMeshComponent->GetNumMaterials(); i++)
				Component->SetMaterial(i, SplineMeshComponent->GetMaterial(i));
			Component->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
			Component->AttachTo(Hologram->GetRootComponent());
			Component->RegisterComponent();
		}
		else SplineMeshComponent->SetStartAndEnd(Data.StartPos, Data.StartTangent, Data.EndPos, Data.EndTangent);
		NeedNew = true;
	}
	
	return Hologram;
}

AFGBuildable* UFSRailroadTrackOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	FVector RelativeVector = TransformOperator.SourceTransform.InverseTransformPositionNoScale(Source->GetTransform().GetLocation());
	FQuat RelativeRotation = TransformOperator.SourceTransform.InverseTransformRotation(Source->GetTransform().GetRotation());
	FQuat Rotation = TransformOperator.TargetTransform.TransformRotation(RelativeRotation);

	FTransform Transform = FTransform(FRotator::ZeroRotator, TransformOperator.TargetTransform.TransformPositionNoScale(RelativeVector), Source->GetTransform().GetScale3D());

	AFGBuildable* Buildable = BuildableSubsystem->BeginSpawnBuildable(Source->GetClass(), Transform);

	AFGBuildableRailroadTrack* SourceTrack = Cast<AFGBuildableRailroadTrack>(Source);
	AFGBuildableRailroadTrack* TargetTrack = Cast<AFGBuildableRailroadTrack>(Buildable);

	TargetTrack->mSplineData.Reset();
	for (const FSplinePointData& PointData : SourceTrack->mSplineData) {
		FSplinePointData NewPointData;
		NewPointData.Location = Rotation.RotateVector(PointData.Location);
		NewPointData.ArriveTangent = Rotation.RotateVector(PointData.ArriveTangent);
		NewPointData.LeaveTangent = Rotation.RotateVector(PointData.LeaveTangent);
		TargetTrack->mSplineData.Add(NewPointData);
	}

	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	if (!Recipe) return nullptr;

	Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());
	Buildable->FinishSpawning(Transform);

	AFGRailroadSubsystem* RailroadSubsystem = AFGRailroadSubsystem::Get(this);
	//RailroadSubsystem->RemoveTrack(TargetTrack);
	//TargetTrack->mTrackGraphID = INDEX_NONE;

	return Buildable;
}

void UFSRailroadTrackOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
	UFGRailroadTrackConnectionComponent* SourceComponent = Cast<UFGRailroadTrackConnectionComponent>(SFC);
	UFGRailroadTrackConnectionComponent* TargetComponent = Cast<UFGRailroadTrackConnectionComponent>(TFC);

	if (!SourceComponent || !TargetComponent) return;
	AFGBuildableRailroadTrack* TargetTrack = Cast<AFGBuildableRailroadTrack>(Buildable);

	if (!TargetComponent->IsConnected() && (SourceComponent->IsConnected() || Force)) {
		TArray<UFGRailroadTrackConnectionComponent*> Result;
		UFGRailroadTrackConnectionComponent* Connection = nullptr;
		if (SourceComponent->IsConnected()) {
			Connection = this->ConnectionMapping<UFGRailroadTrackConnectionComponent>(SourceComponent->GetConnection());
			if (!Connection) {
				FTransform Transform = TransformOperator.Transform(SourceComponent->GetConnection()->GetComponentTransform());
				FindOverlappingConnections(Result, TargetComponent, Transform.GetLocation(), 50.0f);
				if (Result.Num()) Connection = Result[0];
			}
		}
		if (!Connection && Force) {
			FindOverlappingConnections(Result, TargetComponent, TargetComponent->GetComponentTransform().GetLocation(), 50.0f);
			if (Result.Num()) Connection = Result[0];
		}
		if (Connection && Connection != TargetComponent && !Connection->IsConnected()) {
			TargetComponent->AddConnection(Connection);
			AFGRailroadSubsystem* RailroadSubsystem = AFGRailroadSubsystem::Get(this);
			RailroadSubsystem->RemoveTrack(TargetTrack);
			RailroadSubsystem->AddTrack(TargetTrack);
		}
	}
	/*
	SML::Logging::info(*SourceComponent->GetFullName());
	SML::Logging::info(*SourceComponent->GetStation()->GetFullName());
	SML::Logging::info(*TargetComponent->GetStation()->GetFullName());*/
}

void UFSRailroadTrackOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	AFGBuildableRailroadTrack* SourceTrack = Cast<AFGBuildableRailroadTrack>(Source);
	AFGBuildableRailroadTrack* TargetTrack = Cast<AFGBuildableRailroadTrack>(Buildable);

	AFGRailroadSubsystem* RailroadSubsystem = AFGRailroadSubsystem::Get(this);
	//RailroadSubsystem->AddTrack(TargetTrack);
}


FSBuildableType UFSRailroadTrackOperator::GetType() const
{
	return FSBuildableType::Rail;
}

void UFSRailroadTrackOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	for (UActorComponent* TargetConnection : TargetComponent)
		if (Cast<UFGRailroadTrackConnectionComponent>(TargetConnection)) {
			UFGRailroadTrackConnectionComponent* TFC = Cast<UFGRailroadTrackConnectionComponent>(TargetConnection);
			TArray<UFGRailroadTrackConnectionComponent*> Connections = TFC->GetConnections();
			for (UFGRailroadTrackConnectionComponent* Component : Connections) {
				//AFGBuildable* Connection = Cast<AFGBuildable>(Component->GetAttachmentRootActor());
				AFGBuildable* Connection = Cast<AFGBuildable>(TFC->GetConnection()->GetAttachmentRootActor());
				if (Connection) List.Add(Connection);
			}
			if (TFC->GetStation()) List.Add(TFC->GetStation());
		}
}


void UFSRailroadTrackOperator::FindOverlappingConnections(TArray<UFGRailroadTrackConnectionComponent*>& Result, UFGRailroadTrackConnectionComponent* Ignored, const FVector& Loc, const float& Radius)
{
	float RadiusSqr = Radius * Radius;
	for (TObjectIterator<UFGRailroadTrackConnectionComponent> Itr; Itr; ++Itr) {
		UFGRailroadTrackConnectionComponent* Component = *Itr;
		if (Component != Ignored) {
			float DistSqr = (Component->GetComponentLocation() - Loc).SizeSquared();
			if (DistSqr < RadiusSqr) {
				AFGBuildableRailroadTrack* Track = Cast<AFGBuildableRailroadTrack>(Component->GetAttachmentRootActor());
				if (Track && !Track->IsPendingKillOrUnreachable()) Result.Add(Component);
			}
		}
	}
}