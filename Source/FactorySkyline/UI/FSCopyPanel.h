// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UI/FSFoldWidgetBase.h"
#include "FSDesign.h"
#include "Blueprint/UserWidget.h"
#include "FSCopyPanel.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSCopyPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSCopyPanel(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void onCheckBoxChanged(bool IsChecked);

	UFUNCTION()
	void onFullPreviewChanged(bool IsChecked);

	UFUNCTION()
	void onSetItem();

	UFUNCTION()
	void onStartCopy();

	void Init();
	void Load(UFSDesign* Design);

	void Fold();
	void Expand();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SetItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartCopyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* SetItemStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UFSFoldWidgetBase* FoldPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* CheckBoxFullPreview;

	UFSDesign* Design = nullptr;
};
