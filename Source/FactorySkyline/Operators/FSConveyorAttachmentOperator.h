// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/Operators/FSFactoryOperator.h"
#include "FSConveyorAttachmentOperator.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSConveyorAttachmentOperator : public UFSFactoryOperator
{
	GENERATED_BODY()
public:
	virtual void ApplyConnection(AFGBuildable* Buildable, UFGFactoryConnectionComponent* SFC, UFGFactoryConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force);
	virtual void ApplyConnection(AFGBuildable* Buildable, const FSTransformOperator& TransformOperator, bool Force);
	virtual FSBuildableType GetType() const;
	virtual void GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable> >& List) const;

	void TrySplitConveyor(AFGBuildable* Buildable, UFGFactoryConnectionComponent* TFC0, UFGFactoryConnectionComponent* TFC1);
};
