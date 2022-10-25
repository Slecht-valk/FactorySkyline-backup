// ILikeBanas


#include "FSKeySettingEntry.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSInput.h"


UFSKeySettingEntry::UFSKeySettingEntry(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSKeySettingEntry::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	this->KeyDesc->SetIsEnabled(false);

	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	FSkyline->FSCtrl->FSInput->Scaner.Unload(this);
}

FReply UFSKeySettingEntry::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		this->KeyDesc->SetIsEnabled(true);
		//this->SetKeyboardFocus();
		
		AFSkyline* FSkyline = AFSkyline::Get(this);
		if (FSkyline) FSkyline->FSCtrl->FSInput->Scaner.Load(this);
	}

	return FReply::Handled();
}

void UFSKeySettingEntry::Init()
{
	this->KeyDesc->SetIsEnabled(false);
	this->bIsFocusable = true;
}
