// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSMenuWidget.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FSListMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSListMenuWidget : public UFSMenuWidget
{
	GENERATED_BODY()
public:

	UFSListMenuWidget(const FObjectInitializer& ObjectInitializer);

	FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* RootPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ListPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Title;

	virtual void NativeTick(const FGeometry& Geometry, float dt) override;

	virtual void Refresh(float dt);

	const float MaxSpeed = 1200.0f;
	const float MinSpeed = 400.0f;
};
