// ILikeBanas


#include "FSInventoryEntry.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSController.h"
#include "FactorySkyline/FSInventory.h"


FReply UFSInventoryEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UFSInventoryEntry::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		AFSkyline* FSkyline = AFSkyline::Get(this);
		if (FSkyline->FSCtrl->GetPlayer())
			FSkyline->FSCtrl->Inventory.TakeOut(FSkyline->FSCtrl->GetPlayer()->GetInventory(), this->ItemClass);
	}
	return FReply::Handled();
}

FReply UFSInventoryEntry::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseMove(this, InMouseEvent);

	return FReply::Handled();
}

void UFSInventoryEntry::Init(TSubclassOf<UFGItemDescriptor> ItemClass, UTexture2D* Texture)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	this->SkylineUI = FSkyline->SkylineUI;
	this->ItemClass = ItemClass;

	this->DefaultTexture = Texture;
	this->DefaultTextureOnHovered = Texture;
	this->IsEnabled = true;

	UpdateView();
}

void UFSInventoryEntry::SetCount(int Count)
{
	this->MenuText->SetText(FText::FromString(FString::Printf(TEXT(" X %d"), Count)));
}