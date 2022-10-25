// ILikeBanas


#include "FSStatusEntry.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "Resources/FGBuildingDescriptor.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGRecipe.h"

FReply UFSStatusEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UFSStatusEntry::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!BuildableClass->IsChildOf<AFGBuildableFactory>()) return FReply::Handled();
	/*
	for (TWeakObjectPtr<AFGBuildable> Ptr : Design->BuildableSet) {
		AFGBuildable* Buildable = Ptr.Get();
		if (Buildable && Buildable->GetClass() == this->BuildableClass && Buildable->IsUseable()) {
			AFSkyline* FSkyline = AFSkyline::Get(this);
			Buildable->OnUse_Implementation(FSkyline->Player, *(FUseState*)&UFGUseState_ReviveValid());
			break;
		}
	}*/

	return FReply::Handled();
}

FReply UFSStatusEntry::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseMove(this, InMouseEvent);
	
	return FReply::Handled();
}

void UFSStatusEntry::Init(UFSDesign* Design, TSubclassOf<class AFGBuildable> BuildableClass, int Count, UTexture2D* Texture)
{
	this->BuildableClass = BuildableClass;
	this->Count = Count;
	this->Design = Design;

	this->MenuText->SetText(FText::FromString(FString::Printf(TEXT(" X %d"), Count)));

	this->DefaultTexture = Texture;
	this->DefaultTextureOnHovered = Texture;
	this->IsEnabled = true;

	UpdateView();
}
