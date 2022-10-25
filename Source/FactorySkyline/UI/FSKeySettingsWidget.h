// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "FactorySkyline/FSInput.h"
#include "FactorySkyline/UI/FSKeySettingEntry.h"
#include "FSKeySettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSKeySettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSKeySettingsWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* RootPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* MainPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ResetButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ApplyButton;

	UFUNCTION()
	void onExit();

	UFUNCTION()
	void onReset();

	UFUNCTION()
	void onApply();

	void Init();
	void Start();
	void AddEntry(const FName& Action);
	void UpdateKey(const FName& Action, const FSKey& Key, const FText& Desc);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UPROPERTY()
	TMap<FName, UFSKeySettingEntry*> EntryMapping;
};
