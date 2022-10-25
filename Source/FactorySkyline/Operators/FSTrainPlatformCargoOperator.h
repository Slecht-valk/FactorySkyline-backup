// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSTrainPlatformOperator.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSTrainPlatformCargoOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSTrainPlatformCargoOperator : public UFSTrainPlatformOperator
{
	GENERATED_BODY()
public:

	virtual void ApplySettingsTo(AFGBuildable* Buildable);
};
