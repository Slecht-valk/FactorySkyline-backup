// ILikeBanas


#include "FactorySkyline/UI/FSListMenuWidget.h"
#include "FactorySkyline/UI/FSButtonWidget.h"
#include "FactorySkyline/FSMenuItem.h"
#include "FactorySkyline/FSkyline.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"


UFSListMenuWidget::UFSListMenuWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

FReply UFSListMenuWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSDesignMenu* ParentItem = Cast<UFSDesignMenu>(this->ParentItem);
	if (!ParentItem) return FReply::Handled();

	if (!ParentItem->Folded)
		ParentItem->MoveToY -= ParentItem->EntryHeight * InMouseEvent.GetWheelDelta() * 3.0f;

	return FReply::Handled();
}

void UFSListMenuWidget::NativeTick(const FGeometry& Geometry, float dt)
{
	Refresh(dt);
	Super::NativeTick(Geometry, dt);
}

void UFSListMenuWidget::Refresh(float dt)
{
	UFSListMenu* ParentItem = (UFSListMenu*)this->ParentItem;
	
	if (FMath::Abs(ParentItem->CurrentY - ParentItem->MoveToY) > 1e-4) {
		float Speed = FMath::Abs(ParentItem->CurrentY - ParentItem->MoveToY) * 2.0;
		if (Speed < MinSpeed) Speed = MinSpeed;
		if (Speed > MaxSpeed) Speed = MaxSpeed;
		if (ParentItem->CurrentY > ParentItem->MoveToY) Speed = -Speed;

		if (FMath::Abs(ParentItem->CurrentY - ParentItem->MoveToY) > FMath::Abs(Speed * dt))
			ParentItem->CurrentY += Speed * dt;
		else ParentItem->CurrentY = ParentItem->MoveToY;
	}
	else ParentItem->CurrentY = ParentItem->MoveToY;

	if (ParentItem->CurrentY < 0.0f)
		ParentItem->CurrentY = ParentItem->MoveToY = 0.0f;

	float Height = ParentItem->Elements.Num() * ParentItem->EntryHeight - ParentItem->WidgetHeight;
	if (Height < 1e-4) Height = 0.0f;
	if (ParentItem->CurrentY > Height)
		ParentItem->CurrentY = ParentItem->MoveToY = Height;

	if (LastY == ParentItem->CurrentY && !ParentItem->DirtyWidget) return;
	LastY = ParentItem->CurrentY;
	ParentItem->DirtyWidget = false;

	int Index = 0;
	float StartY = ParentItem->CurrentY;
	for (; StartY >= ParentItem->EntryHeight * 100; StartY -= ParentItem->EntryHeight * 100) Index += 100;
	for (; StartY >= ParentItem->EntryHeight * 10; StartY -= ParentItem->EntryHeight * 10) Index += 10;
	for (; StartY >= ParentItem->EntryHeight; StartY -= ParentItem->EntryHeight) Index++;
	StartY = -StartY;

	TSet<UFSMenuEntryBase*> CurrentSet;
	while (StartY < ParentItem->WidgetHeight) {
		if (Index >= ParentItem->Elements.Num()) {
			break;
		}

		UFSMenuItem* Item = Cast<UFSMenuItem>(ParentItem->Elements[Index]);
		UFSMenuEntryBase* ButtonWidget = Cast<UFSMenuEntryBase>(Item->AccquireEntry());
		if (!EntrySet.Contains(ButtonWidget)) {
			ListPanel->AddChild(ButtonWidget);
			ParentItem->InitEntryStyle(ButtonWidget);
		}
		CurrentSet.Add(ButtonWidget);
		//SML::Logging::info(y, TEXT(" "), Index);
		ButtonWidget->SetPosition(0.0f, StartY);

		StartY += ButtonWidget->GetEntryHeight();
		Index++;
	}
	
	for (UFSMenuEntryBase* Entry : EntrySet) if (!CurrentSet.Contains(Entry))
		ListPanel->RemoveChild(Entry);

	EntrySet = CurrentSet;
	if (EntrySet.Num() == 0) Title->SetVisibility(ESlateVisibility::Visible);
	else Title->SetVisibility(ESlateVisibility::Collapsed);

	return;
}

