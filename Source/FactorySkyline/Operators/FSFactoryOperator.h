// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSBuildableOperator.h"
#include "FSFactoryOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSFactoryOperator : public UFSBuildableOperator
{
	GENERATED_BODY()
public:

	virtual void ApplyConnection(AFGBuildable* Target, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplySettingsTo(AFGBuildable* Buildable);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;

};