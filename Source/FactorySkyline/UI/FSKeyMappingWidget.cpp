// ILikeBanas


#include "FactorySkyline/UI/FSKeyMappingWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBoxSlot.h"


UFSKeyMappingWidget::UFSKeyMappingWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSKeyMappingWidget::SetTitle(const FText& Title)
{
	this->Title->SetText(Title);
}

void UFSKeyMappingWidget::SetKey(const FText& Key)
{
	this->Key->SetText(Key);
}

void UFSKeyMappingWidget::SetPadding(const float& Padding)
{
	UHorizontalBoxSlot* Slot = Cast<UHorizontalBoxSlot>(this->Slot);
	Slot->SetPadding(FMargin(Padding, 0.0f));
}

void UFSKeyMappingWidget::SetDefaultView()
{
	HighLight->SetVisibility(ESlateVisibility::Hidden);
	IsHighLight = false;
}

void UFSKeyMappingWidget::SetHighLightView()
{
	HighLight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	IsHighLight = true;
}
