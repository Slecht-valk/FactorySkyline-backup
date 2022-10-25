// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSFactoryOperator.h"
#include "FSTowerOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSTowerOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;
};
