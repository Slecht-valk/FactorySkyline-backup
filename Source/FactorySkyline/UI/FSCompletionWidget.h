// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/FSMenuItem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/UserWidget.h"
#include "FSCompletionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSCompletionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSCompletionWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* UndoButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* OkButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SaveButton;

	UFUNCTION()
	void onUndo();

	UFUNCTION()
	void onOk();

	UFUNCTION()
	void onSave();

	void Init();
	void Load(TWeakObjectPtr<UFSDesignMenu> Source, UFSDesignMenu* Target);

	virtual void NativeTick(const FGeometry& Geometry, float dt) override;

	void StartEnterAnimation();
	void TickEnterAnimation(float dt);

	TWeakObjectPtr<UFSDesignMenu> Source;

	UPROPERTY()
	UFSDesignMenu* Target;

	UCanvasPanelSlot* PanelSlot;
	bool IsPlayingEnterAnimation;
	float AnimationTimeCountDown;
};
