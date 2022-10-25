// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Layout/Geometry.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FactorySkyline/FSMenuItem.h"
#include "FactorySkyline/FSBuildableService.h"
#include "FSLogoWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSLogoWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSLogoWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UUserWidget* Dark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* StartPanel;
	
	UFSGettingStartMenu* GettingStart;
	UFSButtonWidget* StartButton;
	UCanvasPanelSlot* PanelSlot;

	bool IsPlayingEnterAnimation;
	bool IsPlayingFoldAnimation;
	bool IsPlayingFadeAnimation;
	float AnimationTimeCountDown;

	bool IsShow;
	FSTime Time;

	bool Init(UFSGettingStartMenu* GettingStart);

	void ResetLogo();

	void FoldingLogo();

	void ShowLogo();

	void StartEnterAnimation();

	void TickEnterAnimation(float dt);

	void StartFoldAnimation();

	void TickFoldAnimation(float dt);

	void StartFadeAnimation();

	void TickFadeAnimation(float dt);

	void ExpandStart();
	
	virtual void NativeTick(const FGeometry& Geometry, float dt) override;

};
