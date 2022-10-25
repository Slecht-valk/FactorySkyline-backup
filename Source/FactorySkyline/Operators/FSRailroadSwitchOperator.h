// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSRailroadSwitchOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSRailroadSwitchOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:
	virtual AFGHologram* CreateHologram();
	virtual void UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid);
	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
	
};
