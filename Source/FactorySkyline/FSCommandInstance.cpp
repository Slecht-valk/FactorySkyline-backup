// ILikeBanas


#include "FactorySkyline/FSCommandInstance.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSBuilder.h"
#include "FactorySkyline/FSController.h"
#include "UI/FGGameUI.h"
#include "InputCoreTypes.h"
#include "Logging/LogMacros.h"
#include "Command/CommandSender.h"


AFSCommandInstance::AFSCommandInstance() {
	CommandName = TEXT("Skyline");
	Usage = TEXT("/Skyline - Factory Skyline");
}


static void ListAllActor(UCommandSender* Sender)
{
	UWorld* World = Sender->GetWorld();
	for (TActorIterator<AActor>It(World); It; ++It) {
		AActor* Actor = *It;
		UE_LOG(LogSkyline, Display, TEXT("%s"), *Actor->GetFullName());

	}
}

static void ListAllUserWidgetr(UCommandSender* Sender)
{
	UWorld* World = Sender->GetWorld();
	for (TObjectIterator<UUserWidget>It; It; ++It) {
		UUserWidget* Widget = *It;
		UE_LOG(LogSkyline, Display, TEXT("%s"), *Widget->GetFullName());
		UE_LOG(LogSkyline, Display, TEXT("%s"), *Widget->GetName());
	}
}


EExecutionStatus AFSCommandInstance::ExecuteCommand_Implementation(UCommandSender* Sender, const TArray<FString>& Arguments, const FString& Label) {

	AFSkyline* FSkyline = AFSkyline::Get(this);
	FSkyline->SkylineUI->KeySettingsWidget->Start();
	Sender->SendChatMessage(TEXT("Start Key Settings"));
	return EExecutionStatus::COMPLETED;
}