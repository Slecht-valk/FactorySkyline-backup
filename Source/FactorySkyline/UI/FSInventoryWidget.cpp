// ILikeBanas


#include "FSInventoryWidget.h"

#include "FSInventoryEntry.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSInput.h"


UFSInventoryWidget::UFSInventoryWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSInventoryWidget::NativeTick(const FGeometry& Geometry, float dt)
{
	Super::NativeTick(Geometry, dt);

	TMap<TSubclassOf<UFGItemDescriptor>, int> Storage;
	bool Dirty;
	Inventory->GetStorageIfDirty(Storage, Dirty);

	if (Dirty) {
		TArray<TSubclassOf<UFGItemDescriptor>> List;
		EntryCache.GetKeys(List);

		for (TSubclassOf<UFGItemDescriptor>& Key : List)
			if (!Storage.Find(Key))
				RootBox->RemoveChild(EntryCache.FindAndRemoveChecked(Key));

		for (TPair<TSubclassOf<UFGItemDescriptor>, int>& Pair : Storage) {
			UFSInventoryEntry* Entry = AccquireEntry(Pair.Key, Pair.Value);
			if (!EntryCache.Find(Pair.Key)) {
				RootBox->AddChild(Entry);
				EntryCache.Add(Pair.Key, Entry);
			}
		}

	}

	//FSMouseWheel(0.0f);
}

FReply UFSInventoryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseMove(this, InMouseEvent);
	return FReply::Handled();
}

void UFSInventoryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UFSkylineUI* SkylineUI = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUI->FSMouseLeave(this, InMouseEvent);
}


FReply UFSInventoryWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FSMouseWheel(InMouseEvent.GetWheelDelta() * 80.0f);
	return FReply::Handled();
}

void UFSInventoryWidget::FSMouseWheel(float DeltaY)
{
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(RootBox->Slot);
	FVector2D Size = RootBox->GetCachedGeometry().GetLocalSize();
	float Height = RootPanel->GetCachedGeometry().GetLocalSize().Y;
	float y = Slot->GetPosition().Y + DeltaY;

	if (Size.Y <= Height + 1e-4) y = 0.0f;
	else if (0 < y) y = 0.0f;
	else if (y + Size.Y < Height) y = Height - Size.Y;

	if (FMath::Abs(Slot->GetPosition().Y - y) > 1e-4)
		Slot->SetPosition(FVector2D(0.0f, y));
}

void UFSInventoryWidget::Init(FSInventory* Inventory)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	this->SkylineUI = FSkyline->SkylineUI;
	this->Inventory = Inventory;
	FSkyline->FSCtrl->FSInput->BindDelegate("InventoryKey", FSInputEvent::FS_Pressed, this, &UFSInventoryWidget::Toggle);
}

UFSInventoryEntry* UFSInventoryWidget::AccquireEntry(TSubclassOf<UFGItemDescriptor> ItemClass, int Count)
{
	UFSInventoryEntry** Res = EntryMapping.Find(ItemClass);
	if (!Res) {
		TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_InventoryEntry.Widget_InventoryEntry_C"));
		UFSInventoryEntry* Entry = Cast<UFSInventoryEntry>(CreateWidget<UFSInventoryEntry>(this, WidgetClass));
		Entry->Init(ItemClass, UFGItemDescriptor::GetSmallIcon(ItemClass));
		Res = &EntryMapping.Add(ItemClass, Entry);
	}

	(*Res)->SetCount(Count);
	return *Res;
}

void UFSInventoryWidget::Toggle()
{
	if (this->GetVisibility() == ESlateVisibility::Collapsed) {
		AFSkyline* FSkyline = AFSkyline::Get(this);
		FSkyline->SkylineUI->ResetHighLight();
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}