// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSConveyorAttachmentOperator.h"
#include "FSConveyorSmartSplitterOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSConveyorSmartSplitterOperator : public UFSConveyorAttachmentOperator
{
	GENERATED_BODY()
public:

	virtual void ApplySettingsTo(AFGBuildable* Buildable);
};
