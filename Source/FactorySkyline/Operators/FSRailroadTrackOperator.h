// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FGRailroadTrackConnectionComponent.h"
#include "FSRailroadTrackOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSRailroadTrackOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:

	virtual AFGHologram* HologramCopy(FTransform& RelativeTransform);
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
	virtual void ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplySettingsTo(AFGBuildable* Buildable);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;

	void FindOverlappingConnections(TArray<UFGRailroadTrackConnectionComponent*>& Result, UFGRailroadTrackConnectionComponent* Ignored, const FVector& Loc, const float& Radius);
};
