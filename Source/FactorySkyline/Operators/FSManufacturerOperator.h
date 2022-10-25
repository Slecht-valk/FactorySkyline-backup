// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSFactoryOperator.h"
#include "FSManufacturerOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSManufacturerOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:

	//virtual void UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid);
	//virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);

	virtual void ApplySettingsTo(AFGBuildable* Buildable);

};
