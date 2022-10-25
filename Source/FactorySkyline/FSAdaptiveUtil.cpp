// ILikeBanas


#include "FSAdaptiveUtil.h"
#include "FGNetworkLibrary.h"

void UFSAdaptiveUtil::Init()
{
	GameVersion = GetGameVersion();
}

int UFSAdaptiveUtil::GetGameVersion()
{
	/*FString Header;
	FString BuildVersion;
	FString Version = UFGNetworkLibrary::GetLocalBuildVersion();

	Version.Split(TEXT("-"), &Header, &BuildVersion, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	int Ver = FCString::Atoi(*BuildVersion);
	*/
	;
	return FEngineVersion::Current().GetChangelist();
}

UFGFactoryConnectionComponent* UFSAdaptiveUtil::GetConveyorConnection(AFGBuildableConveyorBase* Conveyor, int index)
{
	TSet<UActorComponent*> Set = Conveyor->GetComponents();
	for (UActorComponent* Component : Set) {
		UFGFactoryConnectionComponent* FactoryConnection = Cast<UFGFactoryConnectionComponent>(Component);
		if (FactoryConnection) {
			if (index == 0 && Component->GetName().Equals(TEXT("ConveyorAny0"))) return FactoryConnection;
			if (index == 1 && Component->GetName().Equals(TEXT("ConveyorAny1"))) return FactoryConnection;
		}
	}
	return nullptr;
}

TArray< FSplinePointData >* UFSAdaptiveUtil::GetConveyorBeltSplineData(AFGBuildableConveyorBelt* Conveyor)
{
	if (GameVersion <= 136408) return (TArray< FSplinePointData >*)(((char*)Conveyor) + 1944);
	return &Conveyor->mSplineData;
}


void UFSAdaptiveUtil::CopyConveyorLiftAttribute(AFGBuildableConveyorLift* Source, AFGBuildableConveyorLift* Target)
{
	if (GameVersion <= 136408) {
		/*
		char* a = (char*)Source;
		char* b = (char*)&(Source->mTopTransform);
		char* c = (char*)&(Source->mIsReversed);
		SML::Logging::info(b - a);
		SML::Logging::info(c - a);*/
		FTransform* SourceTransform = (FTransform*)(((char*)Source) + 1904);
		FTransform* TargetTransform = (FTransform*)(((char*)Target) + 1904);
		*TargetTransform = *SourceTransform;
		bool* SourceIsReversed = (bool*)(((char*)Source) + 1952);
		bool* TargetIsReversed = (bool*)(((char*)Target) + 1952);
		*TargetIsReversed = *SourceIsReversed;
	}
	else {
		Target->mTopTransform = Source->mTopTransform;
		Target->mIsReversed = Source->mIsReversed;
	}
}