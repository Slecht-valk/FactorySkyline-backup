// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSFactoryOperator.h"
#include "FSPumpOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSPumpOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:

	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
};