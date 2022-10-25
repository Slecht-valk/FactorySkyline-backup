// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UI/FSFoldWidgetBase.h"
#include "FSDesign.h"
#include "Blueprint/UserWidget.h"
#include "FSDeletePanel.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSDeletePanel : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSDeletePanel(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClearButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* DestroyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* DeleteButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UFSFoldWidgetBase* FoldPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ClearPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* DestroyPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* DeletePanel;

	UFSDesign* Design = nullptr;

	UFUNCTION()
	void onCheckBoxChanged(bool IsChecked);

	UFUNCTION()
	void onClear();

	UFUNCTION()
	void onDestroy();

	UFUNCTION()
	void onRemove();

	void Init();
	void Load(UFSDesign* Design);

	void Fold();
	void Expand();
};
