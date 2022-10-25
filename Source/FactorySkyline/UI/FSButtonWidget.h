// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Math/Color.h"
#include "UI/FSMenuEntryBase.h"
#include "Blueprint/UserWidget.h"
#include "FSButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSButtonWidget : public UFSMenuEntryBase
{
	GENERATED_BODY()
public:
	UFSButtonWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* HighLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MenuText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* RootPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* EditableText;

	virtual void NativeConstruct();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	void OnEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	virtual void SetHightLightView();
	virtual void SetSelectedView();
	virtual void SetDefaultView();

	virtual void SetDefaultTexture();
	virtual void SetHoveredTexture();

	virtual void UpdateView();

	void SetTextPadding(float x);

	void SetTextHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	bool HighLightIcon = false;
	bool Switcher = false;
	TSoftObjectPtr<UTexture2D> DefaultTexture = nullptr;
	TSoftObjectPtr<UTexture2D> DefaultTextureOnHovered = nullptr;
	TSoftObjectPtr<UTexture2D> SwitchTexture = nullptr;
	TSoftObjectPtr<UTexture2D> SwitchTextureOnHovered = nullptr;
};
