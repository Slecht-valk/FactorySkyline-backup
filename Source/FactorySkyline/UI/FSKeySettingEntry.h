// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "FSKeySettingEntry.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSKeySettingEntry : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSKeySettingEntry(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KeyDesc;

	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void Init();

	FString KeyString;
	FName Action;
};
