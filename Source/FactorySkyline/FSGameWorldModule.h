// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Module/GameInstanceModule.h"
#include "FactorySkyline/FSkyline.h"
#include "Module/GameWorldModule.h"
#include "FSGameWorldModule.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AFSkyline* FSSubsystem;
public:
    UFSGameWorldModule();
	
};
#pragma once
