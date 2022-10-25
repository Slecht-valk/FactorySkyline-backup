// ILikeBanas


#include "FSFactoryOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGFactoryLegsComponent.h"
#include "FGPipeConnectionComponent.h"

void UFSFactoryOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
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
				Connection = UFGFactoryConnectionComponent::FindCompatibleOverlappingConnections(TargetComponent, Transform.GetLocation(), 50.0f);
			}
		}
		if (!Connection && Force) {
			Connection = UFGFactoryConnectionComponent::FindCompatibleOverlappingConnections(TargetComponent, TargetComponent->GetComponentTransform().GetLocation(), 50.0f);
		}
		if (Connection && !Connection->IsConnected() && TargetComponent->CanConnectTo(Connection))
			TargetComponent->SetConnection(Connection);
	}
}

void UFSFactoryOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	AFGBuildableFactory* SourceFactory = Cast<AFGBuildableFactory>(Source);
	AFGBuildableFactory* TargetFactory = Cast<AFGBuildableFactory>(Buildable);

	TargetFactory->SetIsProductionPaused(SourceFactory->IsProductionPaused());
}

FSBuildableType UFSFactoryOperator::GetType() const
{
	return FSBuildableType::Factory;
}

void UFSFactoryOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
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
		else if (Cast<UFGPipeConnectionComponentBase>(TargetConnection)) {
			UFGPipeConnectionComponentBase* TPC = Cast<UFGPipeConnectionComponentBase>(TargetConnection);
			if (TPC->GetConnection()) {
				AFGBuildable* Connection = Cast<AFGBuildable>(TPC->GetConnection()->GetAttachmentRootActor());
				if (Connection) List.Add(Connection);
			}
		}
}