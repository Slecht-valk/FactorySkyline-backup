// ILikeBanas


#include "FSConveyorOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableConveyorBase.h"
#include "Buildables/FGBuildableConveyorLift.h"


void UFSConveyorOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
	UFGFactoryConnectionComponent* SourceComponent = Cast<UFGFactoryConnectionComponent>(SFC);
	UFGFactoryConnectionComponent* TargetComponent = Cast<UFGFactoryConnectionComponent>(TFC);

	if (!SourceComponent || !TargetComponent) return;

	if (!TargetComponent->IsConnected() && (SourceComponent->IsConnected() || Force)) {
		UFGFactoryConnectionComponent* Connection = nullptr;
		if (SourceComponent->IsConnected()) {
			Connection = this->ConnectionMapping<UFGFactoryConnectionComponent>(SourceComponent->GetConnection());
			if (!Connection) {
				FTransform Transform = TransformOperator.Transform(SourceComponent->GetConnection()->GetComponentTransform());
				Connection = FindCompatibleConnections(TargetComponent, Transform.GetLocation(), 50.0f);
			}
		}
		if (!Connection && Force) {
			Connection = FindCompatibleConnections(TargetComponent, TargetComponent->GetComponentTransform().GetLocation(), 50.0f);
			/*
			SML::Logging::info(*TargetComponent->GetComponentTransform().GetLocation().ToString());
			SML::Logging::info(*Connection->GetFullName());
			if (Connection)	SML::Logging::info(*Connection->GetComponentLocation().ToString());*/
			if (!Connection && Cast<AFGBuildableConveyorLift>(Buildable))
				Connection = SearchForConveyorLift(TargetComponent);
		}
		if (Connection && !Connection->IsConnected() && TargetComponent->CanConnectTo(Connection))
			TargetComponent->SetConnection(Connection);
	}
}

UFGFactoryConnectionComponent* UFSConveyorOperator::SearchForConveyorLift(UFGFactoryConnectionComponent* TargetComponent)
{
	FVector Location = TargetComponent->GetComponentTransform().GetLocation() + TargetComponent->GetComponentTransform().GetRotation().Vector() * 200.0f;
	UFGFactoryConnectionComponent* Connection = FindCompatibleConnections(TargetComponent, Location, 50.0f);
	if (Connection) return Connection;
	FVector Location2 = TargetComponent->GetComponentTransform().GetLocation() + TargetComponent->GetComponentTransform().GetRotation().Vector() * 400.0f;
	Connection = FindCompatibleConnections(TargetComponent, Location2, 50.0f);
	if (Connection && Cast<AFGBuildableConveyorLift>(Connection->GetAttachmentRootActor())) {
		if ((Connection->GetComponentTransform().GetLocation() + Connection->GetComponentTransform().GetRotation().Vector() * 200.0f - Location).Size() < 50.0f) {
			return Connection;
		}
	}
	return nullptr;
}

FSBuildableType UFSConveyorOperator::GetType() const
{
	return FSBuildableType::Factory;
}

void UFSConveyorOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	for (UActorComponent* TargetConnection : TargetComponent)
		if (Cast<UFGFactoryConnectionComponent>(TargetConnection)) {
			UFGFactoryConnectionComponent* TFC = Cast<UFGFactoryConnectionComponent>(TargetConnection);
			if (TFC->GetConnection()) {
				AFGBuildable* Connection = Cast<AFGBuildable>(TFC->GetConnection()->GetAttachmentRootActor());
				if (Connection) List.Add(Connection);
			}
		}
}

UFGFactoryConnectionComponent* UFSConveyorOperator::FindCompatibleConnections(UFGFactoryConnectionComponent* Component, const FVector& Loc, float Radius)
{
	float RadiusSqr = Radius * Radius;
	if (!Component || !(Component->mDirection == EFactoryConnectionDirection::FCD_INPUT || Component->mDirection == EFactoryConnectionDirection::FCD_OUTPUT)) return nullptr;
	EFactoryConnectionDirection Direction = Component->mDirection == EFactoryConnectionDirection::FCD_INPUT ? EFactoryConnectionDirection::FCD_OUTPUT : EFactoryConnectionDirection::FCD_INPUT;

	for (TObjectIterator<UFGFactoryConnectionComponent> Itr; Itr; ++Itr) {
		UFGFactoryConnectionComponent* TargetComponent = *Itr;
		if (TargetComponent != Component && TargetComponent->mDirection == Direction) {
			float DistSqr = (TargetComponent->GetComponentLocation() - Loc).SizeSquared();
			if (DistSqr < RadiusSqr) {
				AFGBuildable* Buildable = Cast<AFGBuildable>(TargetComponent->GetAttachmentRootActor());
				if (Buildable && !Buildable->IsPendingKillOrUnreachable()) return TargetComponent;
			}
		}
	}
	return nullptr;
}
