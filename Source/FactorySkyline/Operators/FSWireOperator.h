// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSWireOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSWireOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:
	virtual AFGBuildable* CreateCopy(const FSTransformOperator& TransformOperator);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;
};
