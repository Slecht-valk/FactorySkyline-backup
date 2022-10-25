// ILikeBanas


#include "FSTrainStationOperator.h"
#include "Buildables/FGBuildableTrainPlatform.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "FGTrainPlatformConnection.h"
#include "FGRailroadSubsystem.h"

void UFSTrainStationOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	Super::ApplySettingsTo(Buildable);

	AFGBuildableRailroadStation* SourceStation = Cast<AFGBuildableRailroadStation>(Source);
	AFGBuildableRailroadStation* TargetStation = Cast<AFGBuildableRailroadStation>(Buildable);
	
	AFGRailroadSubsystem* RailroadSubsystem = AFGRailroadSubsystem::Get(this); 
	RailroadSubsystem->RemoveTrainStation(TargetStation);

	TargetStation->mRailroadTrack->GetConnection(1)->SetStation(TargetStation);

	/*
	SML::Logging::info(SourceStation->mDockedPlatformList.Num());
	SML::Logging::info(TargetStation->mDockedPlatformList.Num());
	SML::Logging::info(TEXT(""));*/
}
