// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FactorySkyline/Operators/FSFactoryOperator.h"
#include "FSTrainPlatformOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSTrainPlatformOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:

	virtual void ApplyConnection(AFGBuildable* Target, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplySettingsTo(AFGBuildable* Buildable);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;
	
};
