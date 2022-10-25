// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "Resources/FGItemDescriptor.h"

#include "FSInventoryEntry.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSInventoryEntry : public UFSButtonWidget
{
	GENERATED_BODY()
public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	TSubclassOf<UFGItemDescriptor> ItemClass;

	void SetCount(int Count);
	void Init(TSubclassOf<UFGItemDescriptor> ItemClass, UTexture2D* Texture);
};
