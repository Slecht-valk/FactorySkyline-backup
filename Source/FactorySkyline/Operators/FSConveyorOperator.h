// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSConveyorOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSConveyorOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:

	virtual void ApplyConnection(AFGBuildable* Target, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;

	UFGFactoryConnectionComponent* SearchForConveyorLift(UFGFactoryConnectionComponent* TargetInput);
	UFGFactoryConnectionComponent* FindCompatibleConnections(UFGFactoryConnectionComponent* Component, const FVector& Loc, float Radius);

};
