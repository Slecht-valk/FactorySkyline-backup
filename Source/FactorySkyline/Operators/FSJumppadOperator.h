// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSFactoryOperator.h"
#include "FSJumppadOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSJumppadOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:

	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
};