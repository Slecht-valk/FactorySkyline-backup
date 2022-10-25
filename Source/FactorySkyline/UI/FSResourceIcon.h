// ILikeBanas

#pragma once

#include "Resources/FGItemDescriptor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FSResourceIcon.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSResourceIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSResourceIcon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Count;

	void Load(TSubclassOf<UFGItemDescriptor> ItemClass, int Count);
};
