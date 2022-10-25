// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSWalkwayOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSWalkwayOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;

};
