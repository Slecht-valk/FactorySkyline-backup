// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "FSKeyMappingWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSKeyMappingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFSKeyMappingWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* HighLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* Panel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUserWidget* Dark;

	void SetTitle(const FText& Title);
	void SetKey(const FText& Key);
	void SetPadding(const float& Padding);

	void SetDefaultView();
	void SetHighLightView();

	bool IsHighLight = false;
};
