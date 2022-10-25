// ILikeBanas


#include "FSTrainPlatformOperator.h"
#include "Buildables/FGBuildableTrainPlatform.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "FGTrainPlatformConnection.h"
#include "FGRailroadSubsystem.h"


void UFSTrainPlatformOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
	Super::ApplyConnection(Buildable, SFC, TFC, TransformOperator, Force);

	UFGTrainPlatformConnection* SourceComponent = Cast<UFGTrainPlatformConnection>(SFC);
	UFGTrainPlatformConnection* TargetComponent = Cast<UFGTrainPlatformConnection>(TFC);

	if (!SourceComponent || !TargetComponent) return;
	/*
	SML::Logging::info(*SourceComponent->mRailroadTrackConnection->GetFullName());
	SML::Logging::info(*SourceComponent->mConnectedTo->GetFullName());*/

	TargetComponent->mComponentDirection = SourceComponent->mComponentDirection;
	TargetComponent->mPlatformConnectionStatus = SourceComponent->mPlatformConnectionStatus;
	if (SourceComponent->mRailroadTrackConnection) {
		UFGRailroadTrackConnectionComponent* Component = this->ConnectionMapping<UFGRailroadTrackConnectionComponent>(SourceComponent->mRailroadTrackConnection);
		if (Component) TargetComponent->SetRailroadConnectionReference(Component);
		UE_LOG(LogTemp, Verbose, TEXT("%s"), *SourceComponent->mRailroadTrackConnection->GetFullName());
		UE_LOG(LogTemp, Verbose, TEXT("%s"), *TargetComponent->mRailroadTrackConnection->GetFullName());
	}
	if (SourceComponent->mConnectedTo) TargetComponent->mConnectedTo = this->ConnectionMapping<UFGTrainPlatformConnection>(SourceComponent->mConnectedTo);

	if (!TargetComponent->mRailroadTrackConnection && Force) {
		UFGRailroadTrackConnectionComponent* Connection = nullptr;
		Connection = UFGRailroadTrackConnectionComponent::FindOverlappingConnections(nullptr, TargetComponent->GetComponentTransform().GetLocation(), 50.0f, true);
		if (Connection) TargetComponent->SetRailroadConnectionReference(Connection);
	}
}

void UFSTrainPlatformOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	Super::ApplySettingsTo(Buildable);

	AFGBuildableTrainPlatform* SourcePlatform = Cast<AFGBuildableTrainPlatform>(Source);
	AFGBuildableTrainPlatform* TargetPlatform = Cast<AFGBuildableTrainPlatform>(Buildable);

	if (SourcePlatform->mRailroadTrack) {
		AFGBuildable** Track = BuildableMapping->Find(SourcePlatform->mRailroadTrack);
		if (Track) {
			//TargetPlatform->AssignRailroadTrack(Cast<AFGBuildableRailroadTrack>(*Track));
			TargetPlatform->mRailroadTrack = Cast<AFGBuildableRailroadTrack>(*Track);
			TargetPlatform->mRailroadTrack->SetIsOwnedByPlatform(true);
		}
	}

	//if (SourcePlatform->IsOrientationReversed()) TargetPlatform->ReverseConnectionDirections();
	TargetPlatform->mIsOrientationReversed = SourcePlatform->mIsOrientationReversed;

	if (SourcePlatform->mStationDockingMaster) {
		AFGBuildable** Station = BuildableMapping->Find(SourcePlatform->mStationDockingMaster);
		if (Station) {
			TargetPlatform->mStationDockingMaster = Cast<AFGBuildableRailroadStation>(*Station);

		}
	}

	TargetPlatform->mPlatformDockingStatus = SourcePlatform->mPlatformDockingStatus;
	TargetPlatform->mSavedDockingStatus = SourcePlatform->mSavedDockingStatus;

	TargetPlatform->UpdateDockingSequence();
}

FSBuildableType UFSTrainPlatformOperator::GetType() const
{
	return FSBuildableType::Rail;
}
void UFSTrainPlatformOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	AFGBuildableTrainPlatform* TargetPlatform = Cast<AFGBuildableTrainPlatform>(Buildable);
	if (TargetPlatform->mRailroadTrack) List.Add(TargetPlatform->mRailroadTrack);
	if (TargetPlatform->mPlatformConnection0->GetConnectedTo()) {
		AFGBuildableTrainPlatform* Platform = Cast<AFGBuildableTrainPlatform>(TargetPlatform->mPlatformConnection0->GetConnectedTo()->GetAttachmentRootActor());
		if (Platform) List.Add(Platform);
	}
	if (TargetPlatform->mPlatformConnection1->GetConnectedTo()) {
		AFGBuildableTrainPlatform* Platform = Cast<AFGBuildableTrainPlatform>(TargetPlatform->mPlatformConnection0->GetConnectedTo()->GetAttachmentRootActor());
		if (Platform) List.Add(Platform);
	}
	
}