// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FSBuildingOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSBuildingOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:
	virtual FSBuildableType GetType() const;

};
