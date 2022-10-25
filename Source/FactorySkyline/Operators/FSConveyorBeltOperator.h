// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSConveyorOperator.h"
#include "FSConveyorBeltOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSConveyorBeltOperator : public UFSConveyorOperator
{
	GENERATED_BODY()
public:

	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);

};
