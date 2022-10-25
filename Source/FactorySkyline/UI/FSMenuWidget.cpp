// ILikeBanas


#include "FactorySkyline/UI/FSMenuWidget.h"
#include "FactorySkyline/UI/FSkylineUI.h"
#include "FactorySkyline/FSMenuItem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UFSMenuWidget::UFSMenuWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UFSMenuWidget::Load(UObject* Item)
{
	UFSMenuItem* MenuItem = Cast<UFSMenuItem>(Item);
	if (MenuItem) this->SkylineUI = MenuItem->SkylineUI;
	this->ParentItem = Item;
	this->EntrySet.Empty();
	this->LastY = -1e6;
}

void UFSMenuWidget::Unload()
{
	this->EntrySet.Empty();
}

void UFSMenuWidget::SetPosition(FVector2D Size)
{
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->Slot);
	if (Slot) {
		Slot->SetPosition(Size);
	}
}

void UFSMenuWidget::SetSize(FVector2D Size)
{
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->Slot);
	if (Slot) {
		Slot->SetSize(Size);
	}
}


void UFSMenuWidget::StartAnimation()
{
	IsPlayingAnimation = true;
	AnimationTimeCountDown = 0.15;
	this->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 0.0));
}

void UFSMenuWidget::TickAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		this->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 1.0));
		IsPlayingAnimation = false;
		return;
	}
	float Color = 1.0 - AnimationTimeCountDown / 0.2;
	this->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, Color));
}

void UFSMenuWidget::NativeTick(const FGeometry& Geometry, float dt)
{
	Super::NativeTick(Geometry, dt);
	if (IsPlayingAnimation) {
		TickAnimation(dt);
	}
}

FReply UFSMenuWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseMove(this, InMouseEvent);
	return FReply::Handled();
}

void UFSMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseLeave(this, InMouseEvent);
}

FReply UFSMenuWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSDesignMenu* ParentItem = Cast<UFSDesignMenu>(this->ParentItem);
	if (!ParentItem) return FReply::Handled();

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		if (ParentItem->Folded) {
			if (Cast<UFSDesignMenu>(ParentItem->CurrentExpandedItem)) {
				UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
				SkylineUI->UnexpandStart(false);
				Cast<UFSDesignMenu>(ParentItem->CurrentExpandedItem)->Unfold();
			}
		}
		else return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
}

FReply UFSMenuWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UFSMenuWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D CusorPos = InDragDropEvent.GetScreenSpacePosition();
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->Slot);
	UFSDesignMenu* Item = Cast<UFSDesignMenu>(this->ParentItem);
	if (CusorPos.X < Slot->GetPosition().X * Scale) {
		UFSDesignMenu* ParentItem = Cast<UFSDesignMenu>(Item->ParentItem);
		if (ParentItem && !ParentItem->Folded) {
			UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
			SkylineUI->UnexpandStart(false);
			DragFold = true;
			FScriptDelegate Func;
			Func.BindUFunction(this, FName("FSDragCancel"));
			InOperation->OnDragCancelled.AddUnique(Func);
		}
	}
	else if ((Slot->GetPosition().X + Slot->GetSize().X) * Scale < CusorPos.X) {
		UFSDesignMenu* ParentItem = Cast<UFSDesignMenu>(Item->ParentItem);
		if (ParentItem && ParentItem->Folded) {
			UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
			SkylineUI->UnexpandStart(false);
			DragFold = false;
			FScriptDelegate Func;
			Func.BindUFunction(this, FName("FSDragCancel"));
			InOperation->OnDragCancelled.AddUnique(Func);
		}
	}
}

void UFSMenuWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = NewObject<UDragDropOperation>(this);
}

void UFSMenuWidget::FSDragCancel(UDragDropOperation* Operation)
{
	UFSDesignMenu* Item = Cast<UFSDesignMenu>(this->ParentItem);
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->Slot);
	FVector2D CurrentPos = Slot->GetPosition();
	FVector2D CusorPos = UWidgetLayoutLibrary::GetMousePositionOnPlatform() / UWidgetLayoutLibrary::GetViewportScale(this);

	if (DragFold) {
		if (CusorPos.X < Slot->GetPosition().X) Item->Fold();
	}
	else {
		if (Slot->GetPosition().X + Slot->GetSize().X < CusorPos.X) Item->Unfold();
	}
}
