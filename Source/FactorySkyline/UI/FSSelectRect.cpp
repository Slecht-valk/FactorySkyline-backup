// ILikeBanas


#include "FSSelectRect.h"
#include "FactorySkyline/FSkyline.h"


UFSSelectRect::UFSSelectRect(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSSelectRect::Init()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	this->FSCtrl = Skyline->FSCtrl;

	PanelSlot = Cast<UCanvasPanelSlot>(Box->Slot);
}

FReply UFSSelectRect::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		AFSController* FSCtrl = (AFSController*)this->FSCtrl; 
		if (FSCtrl->RectangleSelectMode) {
			if (!FSCtrl->RectangleSelecting)
				FSCtrl->StartRectangleSelect(InMouseEvent.GetScreenSpacePosition() - InGeometry.GetAbsolutePosition(), InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton);
			else FSCtrl->EndRectangleSelect(false);
		}
	}
	return FReply::Handled();
}

FReply UFSSelectRect::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UFSSelectRect::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		AFSController* FSCtrl = (AFSController*)this->FSCtrl;
		if (FSCtrl->RectangleSelectMode) {
			if (FSCtrl->RectangleSelecting)
				FSCtrl->EndRectangleSelect(FSCtrl->RectanglePositive == (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton));
		}
	}

	return FReply::Handled();
}

FReply UFSSelectRect::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	AFSController* FSCtrl = (AFSController*)this->FSCtrl;
	if (FSCtrl->RectangleSelecting) {
		FSCtrl->onMouseMove(InMouseEvent.GetScreenSpacePosition() - InGeometry.GetAbsolutePosition());
	}
	return FReply::Handled();
}