// ILikeBanas


#include "FactorySkyline/UI/FSLogoWidget.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FactorySkyline/UI/FSkylineUI.h"
#include "FactorySkyline/FSkyline.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "FactorySkyline/FSMenuItem.h"

UFSLogoWidget::UFSLogoWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

bool UFSLogoWidget::Init(UFSGettingStartMenu* Start)
{
	PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	GettingStart = Start;

	ResetLogo();

	UFSMenuEntryBase* Entry = Start->AccquireEntry();
	StartPanel->AddChild(Entry);
	Entry->SetEntryHeight(54.0);
	Entry->IsEnabled = false;
	StartButton = Cast<UFSButtonWidget>(Entry);

	IsPlayingEnterAnimation = false;
	IsPlayingFoldAnimation = false;

	return true;
}

void UFSLogoWidget::ResetLogo()
{
	IsPlayingEnterAnimation = false;
	IsPlayingFadeAnimation = false;
	IsPlayingFoldAnimation = false;
	PanelSlot->SetPosition(FVector2D(-480.0, 198.0));
	PanelSlot->SetSize(FVector2D(480.0, 54.0));
}
void UFSLogoWidget::FoldingLogo()
{
	IsPlayingEnterAnimation = false;
	IsPlayingFadeAnimation = false;
	IsPlayingFoldAnimation = false;
	PanelSlot->SetPosition(FVector2D(0, 198.0));
	PanelSlot->SetSize(FVector2D(480.0, 54.0));
}
void UFSLogoWidget::ShowLogo()
{
	IsPlayingEnterAnimation = false;
	IsPlayingFadeAnimation = false;
	IsPlayingFoldAnimation = false;
	PanelSlot->SetPosition(FVector2D(0.0, 198.0));
	PanelSlot->SetSize(FVector2D(326.0f, 54.0f));
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Dark->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));
	IsShow = true;
}

void UFSLogoWidget::StartEnterAnimation()
{
	ResetLogo();
	IsPlayingEnterAnimation = true;
	AnimationTimeCountDown = 0.2;
}

void UFSLogoWidget::TickEnterAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		PanelSlot->SetPosition(FVector2D(0.0, 198.0));
		IsPlayingEnterAnimation = false;
		ExpandStart();
		return;
	}
	PanelSlot->SetPosition(FVector2D(-480.0*AnimationTimeCountDown/0.2, 198.0));
}

void UFSLogoWidget::StartFoldAnimation()
{
	FoldingLogo();
	IsPlayingFoldAnimation = true;
	AnimationTimeCountDown = 3.0;
}

void UFSLogoWidget::TickFoldAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		IsPlayingFoldAnimation = false;
		ShowLogo();
		return;
	}
	if (AnimationTimeCountDown < 1.0) {
		PanelSlot->SetSize(FVector2D(326.4f + 153.6f * AnimationTimeCountDown / 1.0f, 54.0f));
		Cast<UCanvasPanelSlot>(StartButton->Slot)->SetPosition(FVector2D(-153.6f + 153.6f * AnimationTimeCountDown / 1.0f, 0.0f));
	}
}

void UFSLogoWidget::StartFadeAnimation()
{
	IsPlayingFadeAnimation = true;
	AnimationTimeCountDown = 5.0;

	IsShow = false;
}

void UFSLogoWidget::TickFadeAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		this->SetVisibility(ESlateVisibility::Hidden);
		IsPlayingFadeAnimation = false;
		return;
	}
	Dark->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f * sqrtf(AnimationTimeCountDown / 5.0f)));
}

void UFSLogoWidget::ExpandStart()
{
	if (AFSkyline::Get(this)->SkylineUI->Turtioul) {
		GettingStart->Expand(FVector2D(480.0, 198.0));
	}
	else {
		this->ShowLogo();
		GettingStart->Expand(FVector2D(326.0, 198.0));
	}

}

void UFSLogoWidget::NativeTick(const FGeometry &Geometry, float dt)
{
	//SML::Logging::info(TEXT("NativeTick"));
	Super::NativeTick(Geometry, dt);
	if (IsPlayingEnterAnimation) {
		TickEnterAnimation(dt);
	}
	if (IsPlayingFoldAnimation) {
		TickFoldAnimation(dt);
	}
	if (IsPlayingFadeAnimation) {
		TickFadeAnimation(dt);
	}
}