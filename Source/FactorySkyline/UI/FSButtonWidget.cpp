// ILikeBanas


#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FactorySkyline/UI/FSkylineUI.h"
#include "FactorySkyline/FSMenuItem.h"


UFSButtonWidget::UFSButtonWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FScriptDelegate Func;
	Func.BindUFunction(this, FName("OnEditableTextCommitted"));
	this->EditableText->OnTextCommitted.AddUnique(Func);
}

FReply UFSButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (this->IsEnabled) {
		UFSMenuItem* Item = Cast<UFSMenuItem>(this->Item);
		UFSDesignMenu* Parent = Cast<UFSDesignMenu>(Item->ParentItem);
		if (!Parent || !Parent->Folded) {
			UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
			SkylineUI->FSMouseDown(this, InMouseEvent);
		}
	}
	return FReply::Unhandled();
}

FReply UFSButtonWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UFSButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSMenuItem* Item = Cast<UFSMenuItem>(this->Item);
	UFSDesignMenu* Parent = Cast<UFSDesignMenu>(Item->ParentItem);
	if (!Parent || !Parent->Folded) {
		UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
		if (SkylineUI->FSMouseUp(this, InMouseEvent)) {
			if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
				Item->FSClick();
			}
			else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && this->IsEditable) {
				UFSDesignMenu* Item = Cast<UFSDesignMenu>(this->Item);
				if (this->IsEditable && Item) {
					this->MenuText->SetVisibility(ESlateVisibility::Collapsed);
					this->EditableText->SetText(Item->FileName);
					this->EditableText->SetVisibility(ESlateVisibility::Visible);
					this->EditableText->SetKeyboardFocus();
				}
			}
		}
	}
	return FReply::Handled();
}

FReply UFSButtonWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSMenuItem* Item = Cast<UFSMenuItem>(this->Item);
	UFSDesignMenu* Parent = Cast<UFSDesignMenu>(Item->ParentItem);
	if (!Parent || !Parent->Folded) {
		UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
		SkylineUI->FSMouseMove(this, InMouseEvent);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UFSButtonWidget::OnEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (this->IsEditable) {
		UFSDesignMenu* Item = Cast<UFSDesignMenu>(this->Item);
		if (!Item) return;

		if (CommitMethod != ETextCommit::Type::OnCleared) {
			Item->FileName = Text;
			this->MenuText->SetText(Text);
		}

		this->EditableText->SetVisibility(ESlateVisibility::Collapsed);
		this->MenuText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFSButtonWidget::SetHightLightView()
{
	SetHoveredTexture();
	HighLight->SetColorAndOpacity(FLinearColor(0.783538f, 0.291771f, 0.057805f, 1.0f));
	HighLight->SetVisibility(ESlateVisibility::Visible);
	if (HighLightIcon) Icon->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UFSButtonWidget::SetSelectedView()
{
	SetDefaultTexture();
	HighLight->SetColorAndOpacity(FLinearColor(0.341176, 0.341176, 0.341176, 1.0f));
	HighLight->SetVisibility(ESlateVisibility::Visible);
	if (HighLightIcon) Icon->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UFSButtonWidget::SetDefaultView()
{
	SetDefaultTexture();
	HighLight->SetVisibility(ESlateVisibility::Hidden);
	if (HighLightIcon) Icon->SetColorAndOpacity(FLinearColor(0.783538f, 0.40724f, 0.165132f, 1.0f));
}

void UFSButtonWidget::SetDefaultTexture()
{
	if (Switcher) {
		if (!SwitchTexture.IsNull()) Icon->SetBrushFromSoftTexture(SwitchTexture);
	}
	else {
		if (!DefaultTexture.IsNull()) Icon->SetBrushFromSoftTexture(DefaultTexture);
	}
}

void UFSButtonWidget::SetHoveredTexture()
{
	if (Switcher) {
		if (!SwitchTextureOnHovered.IsNull()) Icon->SetBrushFromSoftTexture(SwitchTextureOnHovered);
	}
	else {
		if (!DefaultTextureOnHovered.IsNull()) Icon->SetBrushFromSoftTexture(DefaultTextureOnHovered);
	}
}

void UFSButtonWidget::UpdateView()
{
	if (!this->IsEnabled) {
		SetDefaultView();
		return;
	}

	UFSMenuItem* Item = Cast<UFSMenuItem>(this->Item);

	if (Item) {
		if (Item->IsMultiSelected) {
			return;
		}
		if (Item->ParentItem && Item->ParentItem->CurrentExpandedItem == Item) {
			SetSelectedView();
			return;
		}
	}

	SetDefaultView();
}

void UFSButtonWidget::SetTextPadding(float x)
{
	if (HorBox->Slot) {
		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(HorBox->Slot);
		if (CanvasPanelSlot) {

			FAnchors Anchors = CanvasPanelSlot->GetAnchors();
			Anchors.Minimum.X = x;
			CanvasPanelSlot->SetAnchors(Anchors);
		}
	}
}

void UFSButtonWidget::SetTextHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	if (MenuText->Slot) {
		UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(MenuText->Slot);
		if (HorizontalBoxSlot) {
			HorizontalBoxSlot->SetHorizontalAlignment(InHorizontalAlignment);
		}
	}
}