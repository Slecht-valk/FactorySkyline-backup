#include "FactorySkylineModule.h"
#include "FSkyline.h"
#include "FSGameWorldModule.h"
#include "FSCommandInstance.h"
#include "command/ChatCommandLibrary.h"
#include "FGGameInstance.h"
#include "FGNetworkLibrary.h"
#include "FGBuildableSubsystem.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Buildables/FGBuildableResourceSink.h"
#include "Patching/NativeHookManager.h"

bool CheckVersion(int Ver)
{
	//return Ver >= 136408;
	return Ver >= 195135;
}

void FFactorySkylineModule::StartupModule() {
	int version = FEngineVersion::Current().GetChangelist();
	if (!CheckVersion(version))
	{
		UE_LOG(LogSkyline, Log, TEXT("Skyline Version missmatch..."));
		return;
	}
	//FScriptDelegate Func1 = FScriptDelegate();
	//FScriptDelegate Func2 = FScriptDelegate();

	UE_LOG(LogSkyline, Log, TEXT("Loading Mod: Factory Skyline..."));
	UFGGameInstance* GameManager = GetMutableDefault<UFGGameInstance>();
	//FSSubsystem->SpawnSubsystem(FSSubsystem, AFSkyline::StaticClass(), TEXT("FSSubsystem"));
	
	SUBSCRIBE_METHOD_VIRTUAL_AFTER(UFGGameInstance::LoadComplete, GameManager, [](UFGGameInstance* FGGameInstance, const float loadTime, const FString& mapName) {
		AFSkyline* Skyline = AFSkyline::Get(FGGameInstance);
		Log("Starting Mod: Factory Skyline...");
		if (Skyline) {
			Log("Skyline instance ok");
			Skyline->InitFullSetup();
			AChatCommandSubsystem* Chat = AChatCommandSubsystem::Get(FGGameInstance);
			if (Chat){
				Chat->RegisterCommand(TEXT("FactorySkyline"), AFSCommandInstance::StaticClass());
			}
		}else{
			Log("Skyline instance missing");
		}
	});
	/*
	SUBSCRIBE_METHOD(AFGBuildableSubsystem::TickFactoryActors, [](CallScope <void(*)(AFGBuildableSubsystem*, float)>& Fun, AFGBuildableSubsystem* BuildableSubsystem, float dt) {
		AFSkyline* Skyline = AFSkyline::Get(BuildableSubsystem);
		if (Skyline && Skyline->FSCtrl) {
			Skyline->FSCtrl->onPreFactoryTick();
		}
	});*/
	AFGBuildableResourceSink* BuildableSinkManager = GetMutableDefault<AFGBuildableResourceSink>();
	
	SUBSCRIBE_METHOD_VIRTUAL(AFGBuildableResourceSink::Factory_CollectInput_Implementation, BuildableSinkManager, [](CallScope <void(*)(AFGBuildableResourceSink*)>& Fun, AFGBuildableResourceSink* ResourceSink) {
		AFSkyline* Skyline = AFSkyline::Get(ResourceSink);
		if (Skyline && Skyline->FSCtrl) {
			Skyline->FSCtrl->onPreResourceSinkTick(ResourceSink);
		}
		//SML::Logging::info(*ResourceSink->GetFullName());
	});
	
}

IMPLEMENT_GAME_MODULE(FFactorySkylineModule, FactorySkyline);