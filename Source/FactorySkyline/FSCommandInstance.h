// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "command/ChatCommandLibrary.h"
#include "FSCommandInstance.generated.h"

/**
 *
 */
UCLASS()
class FACTORYSKYLINE_API AFSCommandInstance : public AChatCommandInstance
{
	GENERATED_BODY()
public:
	AFSCommandInstance();
	EExecutionStatus ExecuteCommand_Implementation(UCommandSender* Sender, const TArray<FString>& Arguments, const FString& Label) override;

};
