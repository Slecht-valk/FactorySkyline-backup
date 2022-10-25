// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/FSMenuItem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/UserWidget.h"
#include "FSRepeatWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSRepeatWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSRepeatWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildOneButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildAllButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RepeatBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Title;

	UFUNCTION()
	void onBuildOne();

	UFUNCTION()
	void onBuildAll();

	UFUNCTION()
	void OnEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void Init();
	void Load();

	void CallBack();
	bool BuildAll;

	virtual void NativeTick(const FGeometry& Geometry, float dt) override;

	void StartEnterAnimation();
	void TickEnterAnimation(float dt);

	int GetCount();
	void SetCount(int Count);

	UCanvasPanelSlot* PanelSlot;
	bool IsPlayingEnterAnimation;
	float AnimationTimeCountDown;
};
