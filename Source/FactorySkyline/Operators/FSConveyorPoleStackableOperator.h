// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSConveyorPoleStackableOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSConveyorPoleStackableOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:

	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;
};
