// ILikeBanas


#include "FSConveyorAttachmentOperator.h"
#include "Buildables/FGBuildableAttachmentMerger.h"
#include "Buildables/FGBuildableAttachmentSplitter.h"


void UFSConveyorAttachmentOperator::ApplyConnection(AFGBuildable* Buildable, UFGFactoryConnectionComponent* SFC, UFGFactoryConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
	if (!TFC->IsConnected() && (SFC->IsConnected() || Force)) {
		UFGFactoryConnectionComponent* Connection = nullptr;
		if (SFC->IsConnected()) {
			Connection = this->ConnectionMapping<UFGFactoryConnectionComponent>(SFC->GetConnection());
			if (!Connection) {
				FTransform Transform = TransformOperator.Transform(SFC->GetConnection()->GetComponentTransform());
				Connection = UFGFactoryConnectionComponent::FindCompatibleOverlappingConnections(TFC, Transform.GetLocation(), 50.0f);
				if (Connection && (Connection->GetAttachmentRootActor()== Buildable || !(Connection->GetComponentQuat().Vector() + TFC->GetComponentQuat().Vector()).IsNearlyZero()))
					Connection = nullptr;
			}
		}
		if (!Connection && Force) {
			Connection = UFGFactoryConnectionComponent::FindCompatibleOverlappingConnections(TFC, TFC->GetComponentTransform().GetLocation(), 50.0f);
		}
		if (Connection && !Connection->IsConnected() && TFC->CanConnectTo(Connection))
			TFC->SetConnection(Connection);
	}
}

void UFSConveyorAttachmentOperator::ApplyConnection(AFGBuildable* Buildable, const FSTransformOperator& TransformOperator, bool Force)
{
	TArray<UActorComponent*> SourceComponent = Source->GetComponentsByClass(UFGFactoryConnectionComponent::StaticClass());
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGFactoryConnectionComponent::StaticClass());
	TArray<UFGFactoryConnectionComponent*> TargetFactoryComponent;

	for (UActorComponent* SourceConnection : SourceComponent) {
		UFGFactoryConnectionComponent* SFC = Cast<UFGFactoryConnectionComponent>(SourceConnection);
		UFGFactoryConnectionComponent* TFC = ConnectionMapping<UFGFactoryConnectionComponent>(SFC);
		if (SFC && TFC) {
			ApplyConnection(Buildable, SFC, TFC, TransformOperator, Force);
			if (!TFC->IsConnected() && (SFC->IsConnected() || Force)) TargetFactoryComponent.Add(TFC);
		}
	}

	for (UFGFactoryConnectionComponent* TFC0 : TargetFactoryComponent)
		for (UFGFactoryConnectionComponent* TFC1 : TargetFactoryComponent)
			if (TFC0->GetDirection() != TFC1->GetDirection() && (TFC0->GetComponentQuat().Vector() + TFC1->GetComponentQuat().Vector()).IsNearlyZero()) {
				TrySplitConveyor(Buildable, TFC0, TFC1);
				return;
			}

}

void UFSConveyorAttachmentOperator::TrySplitConveyor(AFGBuildable* Buildable, UFGFactoryConnectionComponent* TFC0, UFGFactoryConnectionComponent* TFC1)
{
	TArray<FOverlapResult> Result;
	FCollisionShape Shape;
	FTransform Transform = Buildable->GetTransform();

	Shape.SetSphere(50.0f);
	World->OverlapMultiByChannel(Result, Transform.GetLocation(), Transform.GetRotation(), ECollisionChannel::ECC_Visibility, Shape);
	for (FOverlapResult& Hit : Result) {
		AFGBuildableConveyorBelt* ConveyorBelt = Cast<AFGBuildableConveyorBelt>(Hit.GetActor());
		if (ConveyorBelt) {
			float offset = ConveyorBelt->FindOffsetClosestToLocation(Transform.GetLocation());
			TArray< AFGBuildableConveyorBelt* > Belts = AFGBuildableConveyorBelt::Split(ConveyorBelt, offset, false);
			if (Belts.Num() == 2) {
				UFGFactoryConnectionComponent* BC0 = Belts[0]->GetConnection1();
				UFGFactoryConnectionComponent* BC1 = Belts[1]->GetConnection0();
				if (!BC0->IsConnected() && !BC1->IsConnected()) {
					if (BC0->GetDirection() != TFC0->GetDirection()) {
						TFC0->SetConnection(BC0);
						TFC1->SetConnection(BC1);
					}
					else {
						TFC0->SetConnection(BC1);
						TFC1->SetConnection(BC0);
					}
				}
			}
			return;
		}
	}
}

FSBuildableType UFSConveyorAttachmentOperator::GetType() const
{
	return FSBuildableType::Factory;
}

void UFSConveyorAttachmentOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGFactoryConnectionComponent::StaticClass());
	for (UActorComponent* TargetConnection : TargetComponent) {
		UFGFactoryConnectionComponent* TFC = Cast<UFGFactoryConnectionComponent>(TargetConnection);
		if (TFC && TFC->IsConnected()) {
			AFGBuildable* Connection = Cast<AFGBuildable>(TFC->GetConnection()->GetAttachmentRootActor());
			List.Add(Connection);
		}
	}
}