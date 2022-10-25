// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FactorySkyline/FSDesign.h"
#include "FSStatusEntry.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSStatusEntry : public UFSButtonWidget
{
	GENERATED_BODY()
public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	TSubclassOf< class AFGBuildable > BuildableClass;
	int Count;
	UFSDesign* Design = nullptr;

	void Init(UFSDesign* Design, TSubclassOf< class AFGBuildable > BuildableClass, int Count, UTexture2D* Texture);
};
