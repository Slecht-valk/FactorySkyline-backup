// ILikeBanas


#include "FSkyline.h"
#include "UI/FSkylineUI.h"
#include "FSGameWorldModule.h"
#include "Hologram/FGFactoryHologram.h"
#include "FGPlayerController.h"
#include "FGFactoryLegsComponent.h"
#include "FSBuilder.h"
#include "FSSelection.h"
#include "FSController.h"
#include "Misc/AssetRegistryInterface.h"
#include "Operators/FSBuildableOperator.h"
#include "UI/FGGameUI.h"
#include "Subsystem/ModSubsystem.h"
#include "Subsystem/SubsystemActorManager.h"


AFSkyline* AFSkyline::Get(UObject* WorldContext) {
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContext);
	USubsystemActorManager* SubsystemManager = World->GetSubsystem<USubsystemActorManager>();
	return SubsystemManager->GetSubsystemActor<AFSkyline>();

	//Original:
	/*UFSSubsystemHolder* Holder = GetSubsystemHolder<UFSSubsystemHolder>(WorldContext);
	if (Holder == nullptr)
		return nullptr;
	return Holder->GetMySubsystem();
	*/
}

void AFSkyline::Init(){
}

DEFINE_LOG_CATEGORY(LogSkyline);

void AFSkyline::InitFullSetup()
{
	UE_LOG(LogSkyline, Verbose, TEXT("Initializing skyline"));
	this->World = this->GetWorld();
	this->WorldSettings = Cast<AFGWorldSettings>(World->GetWorldSettings());

	this->FGController = World->GetFirstPlayerController<AFGPlayerController>();
	this->HUD = Cast<AFGHUD>(this->FGController->GetHUD());
	this->GameUI = this->HUD->GetGameUI();

	if (!this->FSCtrl) this->FSCtrl = SpawnHiddenActor<AFSController>();
	this->Builder = SpawnHiddenActor<AFSBuilder>();

	this->Select = NewObject<UFSSelection>(this);
	
	this->OperatorFactory = NewObject<UFSOperatorFactory>(this);
	this->SplineHologramFactory = NewObject<UFSSplineHologramFactory>(this);
	this->BuildableService = NewObject<UFSBuildableService>(this);
	this->ConnectSelectService = NewObject<UFSConnectSelectService>(this);
	this->RectSelectService = NewObject<UFSRectSelectService>(this);
	this->AdaptiveUtil = NewObject<UFSAdaptiveUtil>(this);



	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_FSkylineUI.Widget_FSkylineUI_C"));
	this->SkylineUI = Cast<UFSkylineUI>(CreateWidget<UUserWidget>(World, WidgetClass));
	
	this->FSCtrl->Init(); //need first
	this->Builder->Init();
	this->Select->Init();
	this->SkylineUI->Init(World);
	this->OperatorFactory->Init();
	this->SplineHologramFactory->Init();
	this->BuildableService->Init();
	this->ConnectSelectService->Init();
	this->RectSelectService->Init();
	this->AdaptiveUtil->Init();

	IsFullLoaded = true;
	UE_LOG(LogSkyline, Verbose, TEXT("Init done skyline"));
}

static void ListAllUserWidgetr(UWorld* World)
{
	for (TObjectIterator<UUserWidget>It; It; ++It) {
		UUserWidget* Widget = *It;
		UE_LOG(LogSkyline, Display, TEXT("%s"), *Widget->GetFullName());

	}
}

template <class T>
T* AFSkyline::SpawnHiddenActor()
{
	FTransform Transform(FRotator::ZeroRotator, FVector::DownVector * 1e6);
	T* Result = World->SpawnActor<T>(FVector::DownVector * 1e6, FRotator::ZeroRotator);
	Result->SetActorHiddenInGame(true);
	Result->FinishSpawning(Transform);
	return Result;
}
