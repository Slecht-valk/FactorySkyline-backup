// ILikeBanas


#include "FactorySkyline/UI/FSMenuEntryBase.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


UFSMenuEntryBase::UFSMenuEntryBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSMenuEntryBase::SetSelectedView()
{
}

void UFSMenuEntryBase::SetPosition(float x, float y)
{
	if (this->Slot) {
		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
		CanvasPanelSlot->SetPosition(FVector2D(x, y));
	}
}

float UFSMenuEntryBase::GetEntryHeight()
{
	return Height;
}

void UFSMenuEntryBase::SetEntryHeight(float Height)
{
	this->Height = Height;
	if (this->Slot) {
		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);
		if (CanvasPanelSlot) {
			CanvasPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 0.0f));
			CanvasPanelSlot->SetSize(FVector2D(0, Height));
		}
	}
}
