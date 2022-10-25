// ILikeBanas


#include "FSMenuItem.h"
#include "FSDesign.h"
#include "FSkyline.h"
#include "FSController.h"
#include "UI/FSButtonWidget.h"
#include "UI/FSListMenuWidget.h"
#include "UI/FSMenuEntryBase.h"
#include "UI/FSMenuWidget.h"
#include "UI/FSkylineUI.h"
#include "UI/FSOperatingWidget.h"
#include "UI/FSInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

UFSMenuEntryBase* UFSMenuItem::AccquireEntry()
{
	return nullptr;
}

UFSMenuWidget* UFSMenuItem::AccquireMenuWidget()
{
	return nullptr;
}

void UFSMenuItem::ReleaseEntry()
{
}

void UFSMenuItem::ReleaseMenuWidget()
{
}

UFSMenuEntryBase* UFSMenuItem::ConstructEntry()
{
	return nullptr;
}

UFSMenuWidget* UFSMenuItem::ConstructMenuWidget()
{
	return nullptr;
}

void UFSMenuItem::InitEntryStyle(UFSMenuEntryBase* Child)
{
}

void UFSMenuItem::InitWidgetStyle()
{
}

void UFSMenuItem::InitItem()
{
}

void UFSMenuItem::FSClick()
{
}

void UFSMenuItem::ExecClick()
{
}

void UFSMenuItem::Expand()
{
}

void UFSMenuItem::Unexpand()
{
	if (ParentItem) {
		if (!ParentItem->CurrentExpandedItem) return;
		ParentItem->CurrentExpandedItem = nullptr;
	}
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(this->AccquireEntry());
	if (Entry && Entry->IsEnabled) Entry->UpdateView();

	if (this->CurrentExpandedItem) this->CurrentExpandedItem->Unexpand();
	
	UFSOperatingWidget* OperatingWidget = Cast<UFSOperatingWidget>(this->CurrentMenuWidget);
	if (OperatingWidget) OperatingWidget->Unload();
	this->CurrentMenuWidget->RemoveFromParent();
}

void UFSMenuItem::AddChildAt(UFSMenuItem* Child, int32 index)
{
	Elements.Insert(Child, index);
	Child->SkylineUI = this->SkylineUI;
	Child->ParentItem = this;
	Child->InitItem();
}
void UFSMenuItem::RemoveChild(UFSMenuItem* Child)
{
	Elements.Remove(Child);
}
void UFSMenuItem::AddChild(UFSMenuItem* Child)
{
	AddChildAt(Child, Elements.Num());
}

void UFSMenuItem::AddChildAfter(UFSMenuItem* Child, UFSMenuItem* After)
{
	int32 index = Elements.IndexOfByKey(After);
	if (index == INDEX_NONE) return;
	AddChildAt(Child, index + 1);
}

template <typename T>
T* UFSMenuItem::CreateUserWidget(const TCHAR* Class)
{
	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, Class);
	if (!WidgetClass) return nullptr;

	return CreateWidget<T>(this->GetWorld(), WidgetClass);
}


UFSMenuEntryBase* UFSSingleMenu::AccquireEntry()
{
	if (CurrentEntry) return CurrentEntry;
	CurrentEntry = ConstructEntry();
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(CurrentEntry);

	if (!Entry) return CurrentEntry;
	Entry->SkylineUI = this->SkylineUI;
	Entry->Item = this;
	Entry->IsEnabled = true;

	return CurrentEntry;
}

void UFSSingleMenu::FSClick()
{
	this->ExecClick();
}

UFSMenuEntryBase* UFSSwitchMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->DefaultTexture = FSoftObjectPath(TEXT("/FactorySkyline/Icon/SwitchOff.SwitchOff"));
	Entry->DefaultTextureOnHovered = FSoftObjectPath(TEXT("/FactorySkyline/Icon/SwitchOff.SwitchOff"));
	Entry->SwitchTexture = FSoftObjectPath(TEXT("/FactorySkyline/Icon/SwitchOn.SwitchOn"));
	Entry->SwitchTextureOnHovered = FSoftObjectPath(TEXT("/FactorySkyline/Icon/SwitchOn.SwitchOn"));

	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Entry->HorBox->Slot);
	Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Slot->SetOffsets(FMargin(40.0f, 0.0f, 0.0f, 0.0f));

	Entry->Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	InitEntry(Entry);
	return Entry;
}

void UFSSwitchMenu::ExecClick()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(AccquireEntry());
	Entry->Switcher = !Entry->Switcher;
	Entry->UpdateView();
	FSkyline->FSCtrl->Etc->SetBool(AttributeName, Entry->Switcher);
	this->onSwitcherChanged();
}

void UFSSwitchMenu::InitItem()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(AccquireEntry());
	Entry->Switcher = FSkyline->FSCtrl->Etc->GetBool(AttributeName);
	Entry->UpdateView();
	this->onSwitcherChanged();
}

void UFSListMenu::AddChildAt(UFSMenuItem* Child, int32 index)
{
	Super::AddChildAt(Child, index);
	this->Height = EntryHeight * Elements.Num();
	DirtyWidget = true;
}

void UFSListMenu::RemoveChild(UFSMenuItem* Child)
{
	Super::RemoveChild(Child);
	this->Height = EntryHeight * Elements.Num();
	DirtyWidget = true;
}

UFSMenuEntryBase* UFSListMenu::AccquireEntry()
{
	if (CurrentEntry) return CurrentEntry;
	CurrentEntry = ConstructEntry();
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(CurrentEntry);

	if (!Entry) return CurrentEntry;
	Entry->SkylineUI = this->SkylineUI;
	Entry->Item = this;
	Entry->IsEnabled = true;

	return CurrentEntry;
}

UFSMenuWidget* UFSListMenu::AccquireMenuWidget()
{
	DirtyWidget = true;
	if (CurrentMenuWidget) return CurrentMenuWidget;

	CurrentMenuWidget = ConstructMenuWidget();
	UFSListMenuWidget* ListMenu = Cast<UFSListMenuWidget>(CurrentMenuWidget);
	if (!ListMenu) return CurrentMenuWidget;

	ListMenu->Load(this);

	return CurrentMenuWidget;
}

UFSMenuWidget* UFSListMenu::ConstructMenuWidget()
{
	UFSListMenuWidget* ListMenu = CreateUserWidget<UFSListMenuWidget>(TEXT("/FactorySkyline/Widget_FSListMenu.Widget_FSListMenu_C"));
	return ListMenu;
}

void UFSListMenu::InitEntryStyle(UFSMenuEntryBase* Child)
{
	Child->SetEntryHeight(EntryHeight);
}

void UFSListMenu::InitWidgetStyle()
{
	this->WidgetHeight = EntryHeight * Elements.Num();

	CurrentMenuWidget->SetSize(FVector2D(WidgetWidth, WidgetHeight));
}

void UFSListMenu::FSClick()
{
	if (ParentItem && ParentItem->CurrentExpandedItem == this) {
		this->Unexpand();
	}
	else {
		if (ParentItem && ParentItem->CurrentExpandedItem)
			ParentItem->CurrentExpandedItem->Unexpand();
		this->ExecClick();
	}
}

void UFSListMenu::ExecClick()
{
	Expand();
}

void UFSListMenu::MoveTo(UFSMenuItem* Child)
{
	int32 index = Elements.IndexOfByKey(Child);
	if (index == INDEX_NONE) return;
	float MoveTo = index * EntryHeight;

	if (MoveTo < MoveToY) MoveToY = MoveTo;
	else if (MoveToY + WidgetHeight < MoveTo + EntryHeight) MoveToY = MoveTo + EntryHeight - WidgetHeight;
}

void UFSListMenu::Expand()
{
	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(this->AccquireEntry());

	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->ParentItem->AccquireMenuWidget()->Slot);
	Expand(FVector2D(Slot->GetPosition().X + Slot->GetSize().X, Entry ? Cast<UCanvasPanelSlot>(Entry->Slot)->GetPosition().Y + Slot->GetPosition().Y : Slot->GetPosition().Y));

	if (Entry && Entry->IsEnabled) Entry->UpdateView();

	UFSListMenu* Parent = Cast<UFSListMenu>(this->ParentItem);
	Parent->MoveTo(this);
}

void UFSListMenu::Expand(FVector2D Position)
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);

	UFSMenuWidget* MenuWidget = this->AccquireMenuWidget();

	SkylineUIVar->RootPanel->AddChild(MenuWidget);
	MenuWidget->SetPosition(Position);
	this->InitWidgetStyle();
	MenuWidget->StartAnimation();

	if (ParentItem) ParentItem->CurrentExpandedItem = this;
	this->CurrentExpandedItem = nullptr;

}


bool UFSDesignMenu::ShouldSave_Implementation() const
{
	AFSkyline* FSkyline = AFSkyline::Get(this->GetWorld());
	return FSkyline && FSkyline->FSCtrl && FSkyline->FSCtrl->Etc ? !FSkyline->FSCtrl->Etc->GetBool("DontSaveAnything") : false;
}

void UFSDesignMenu::LoadInit(UObject* SkylineUIParam, UFSDesignMenu* Parent)
{
	this->SkylineUI = SkylineUIParam;
	this->ParentItem = Parent;
	this->WidgetWidth = 250.0f;
	this->WidgetHeight = 400.0f;
	this->Height = EntryHeight * Elements.Num();
	for (UFSMenuItem* Child : Elements) {
		UFSDesignMenu* ChildDesign = Cast<UFSDesignMenu>(Child);
		if (ChildDesign) ChildDesign->LoadInit(SkylineUIParam, this);
	}
}

UFSMenuEntryBase* UFSDesignMenu::AccquireEntry()
{
	if (CurrentEntry) return CurrentEntry;
	
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	if (SkylineUIVar->EntryRecycle.Num()) CurrentEntry = SkylineUIVar->EntryRecycle.Pop();
	else CurrentEntry = ConstructEntry();

	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(CurrentEntry);

	if (!Entry) return CurrentEntry;
	Entry->SkylineUI = this->SkylineUI;
	Entry->Item = this;
	Entry->MenuText->SetText(this->FileName);
	Entry->IsEnabled = true;
	Entry->IsEditable = true;

	if (this->Design) {
		Entry->DefaultTexture = FSoftObjectPath(TEXT("/FactorySkyline/Icon/Design.Design"));
		Entry->DefaultTextureOnHovered = FSoftObjectPath(TEXT("/FactorySkyline/Icon/Design.Design"));
	}
	else {
		Entry->DefaultTexture = FSoftObjectPath(TEXT("/FactorySkyline/Icon/Folder.Folder"));
		Entry->DefaultTextureOnHovered = FSoftObjectPath(TEXT("/FactorySkyline/Icon/Folder.Folder"));
	}

	Entry->UpdateView();

	return CurrentEntry;
}

UFSDesignMenu* UFSDesignMenu::CreateDesignMenu(UObject* WorldContext, bool WithDesign)
{
	UFSDesignMenu* DesignMenu = NewObject<UFSDesignMenu>(WorldContext);
	DesignMenu->Design = WithDesign ? NewObject<UFSDesign>(DesignMenu) : nullptr;
	DesignMenu->FileName = FText::FromString(TEXT("Unnamed"));
	DesignMenu->IsFolder = !WithDesign;

	DesignMenu->WidgetWidth = 250.0f;
	DesignMenu->WidgetHeight = 400.0f;

	return DesignMenu;
}

UFSDesignMenu* UFSDesignMenu::CopyFrom(UObject* WorldContext, UFSDesignMenu* Source, bool WithDesign)
{
	UFSDesignMenu* DesignMenu = CreateDesignMenu(WorldContext, WithDesign);
	DesignMenu->FileName = FText::FromString(FString(TEXT("Copy of ")).Append(Source->FileName.ToString()));

	return DesignMenu;
}

UFSMenuEntryBase* UFSDesignMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;

	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Entry->HorBox->Slot);
	Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Slot->SetOffsets(FMargin(40.0f, 0.0f, 0.0f, 0.0f));

	Entry->Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Entry->HighLightIcon = true;

	return Entry;
}

UFSMenuWidget* UFSDesignMenu::AccquireMenuWidget()
{
	if (CurrentMenuWidget) return CurrentMenuWidget;

	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	DirtyWidget = true;

	if (this->Design) {
		return CurrentMenuWidget = SkylineUIVar->OperatingWidget;
	}

	if (SkylineUIVar->WidgetRecycle.Num()) CurrentMenuWidget = SkylineUIVar->WidgetRecycle.Pop();
	else CurrentMenuWidget = ConstructMenuWidget();

	UFSListMenuWidget* ListMenu = Cast<UFSListMenuWidget>(CurrentMenuWidget);
	if (!ListMenu) return CurrentMenuWidget;

	ListMenu->Load(this);

	return CurrentMenuWidget;
}

void UFSDesignMenu::ReleaseEntry()
{
	if (!this->CurrentEntry) return;
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUIVar->EntryRecycle.Add(this->CurrentEntry);
	this->CurrentEntry = nullptr;
}

void UFSDesignMenu::ReleaseMenuWidget()
{
	if (!this->CurrentMenuWidget) return;
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);

	if (!this->Design) {
		this->CurrentMenuWidget->Unload();
		SkylineUIVar->WidgetRecycle.Add(this->CurrentMenuWidget);
	}

	this->CurrentMenuWidget = nullptr;
}

void UFSDesignMenu::InitWidgetStyle()
{
	CurrentMenuWidget->SetSize(FVector2D(WidgetWidth, WidgetHeight));
}

void UFSDesignMenu::ExecClick()
{
	this->Expand();
}

void UFSDesignMenu::Expand()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	SkylineUIVar->UnexpandStart(false);

	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(this->ParentItem->AccquireMenuWidget()->Slot);
	if (this->Design) SkylineUIVar->OperatingWidget->Load(this);
	Expand(FVector2D(Slot->GetPosition().X + Slot->GetSize().X, Slot->GetPosition().Y));

	UFSButtonWidget* Entry = Cast<UFSButtonWidget>(this->AccquireEntry());
	if (Entry && Entry->IsEnabled) Entry->UpdateView();

	UFSListMenu* Parent = Cast<UFSListMenu>(this->ParentItem);
	Parent->MoveTo(this);
}

void UFSDesignMenu::Expand(FVector2D Position)
{
	Super::Expand(Position);
}

void UFSDesignMenu::Unexpand()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	if (this->Design) SkylineUIVar->OperatingWidget->Unload();
	Super::Unexpand();
}

void UFSDesignMenu::Fold()
{
	if (!this->ParentItem) return;
	if (Cast<UFSDesignMenu>(this->ParentItem)->Folded) return;

	UFSMenuWidget* Widget = this->AccquireMenuWidget();
	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	FVector2D Pos = CurrentSlot->GetPosition();

	UFSMenuWidget* ParentWidget = this->ParentItem->AccquireMenuWidget();
	UCanvasPanelSlot* ParentSlot = Cast<UCanvasPanelSlot>(ParentWidget->Slot);

	for (UFSMenuItem* Item = this->ParentItem->ParentItem; Item; Item = Item->ParentItem) {
		if (Item->AccquireMenuWidget()->GetVisibility() == ESlateVisibility::Collapsed) break;
		Item->AccquireMenuWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}

	ParentSlot->SetPosition(FVector2D(0.0f, Pos.Y));
	ParentSlot->SetSize(FVector2D(40.0f, ParentSlot->GetSize().Y));

	CurrentSlot->SetPosition(FVector2D(50.0f, Pos.Y));
	for (UFSMenuItem* Item = this->CurrentExpandedItem; Item; Item = Item->CurrentExpandedItem) {
		UFSMenuWidget* WidgetVar = Item->AccquireMenuWidget();
		UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(WidgetVar->Slot);
		Slot->SetPosition(FVector2D(CurrentSlot->GetPosition().X + CurrentSlot->GetSize().X, Pos.Y));
		CurrentSlot = Slot;
	}

	Cast<UFSDesignMenu>(this->ParentItem)->Folded = true;
}

void UFSDesignMenu::Unfold()
{
	if (!this->ParentItem) return;
	if (!Cast<UFSDesignMenu>(this->ParentItem)->Folded) return;

	UFSMenuWidget* Widget = this->AccquireMenuWidget();
	UCanvasPanelSlot* CurrentSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	FVector2D Pos = CurrentSlot->GetPosition();

	Cast<UFSDesignMenu>(this->ParentItem)->InitWidgetStyle();
	Cast<UFSDesignMenu>(this->ParentItem)->Folded = false;

	UFSMenuItem* Item = this->ParentItem;
	for (; Item->ParentItem; Item = Item->ParentItem) {
		Item->ParentItem->AccquireMenuWidget()->SetVisibility(ESlateVisibility::Visible);
		if (Cast<UFSDesignMenu>(Item->ParentItem)->Folded) break;
	}
	if (!Item->ParentItem) Item = Item->CurrentExpandedItem;
	float x = Cast<UFSDesignMenu>(Item->ParentItem)->Folded ? 50.0f : Cast<UCanvasPanelSlot>(Item->ParentItem->AccquireMenuWidget()->Slot)->GetSize().X;

	CurrentSlot = Cast<UCanvasPanelSlot>(Item->AccquireMenuWidget()->Slot);
	CurrentSlot->SetPosition(FVector2D(x, Pos.Y));
	
	for (Item = Item->CurrentExpandedItem; Item; Item = Item->CurrentExpandedItem) {
		UFSMenuWidget* WidgetVar = Item->AccquireMenuWidget();
		UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(WidgetVar->Slot);
		Slot->SetPosition(FVector2D(CurrentSlot->GetPosition().X + CurrentSlot->GetSize().X, Pos.Y));
		CurrentSlot = Slot;
	}

}

UFSMenuEntryBase* UFSGettingStartMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Getting Start")));
	Entry->SetHightLightView();
	Entry->SetTextPadding(0.05f);
	//Entry->SetTextHorizontalAlignment(EHorizontalAlignment::HAlign_Center);

	return Entry;
}

void UFSGettingStartMenu::InitItem()
{
	this->WidgetWidth = 220.0f;

	//AddChild(Cast<UFSMenuItem>(NewObject<UFSFastConstructMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSNewDesignMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSNewFolderMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSInventoryMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSKeySettingMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSTookitMenu>(this)));
}

UFSMenuEntryBase* UFSFastConstructMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Fast Construct")));
	return Entry;
}

void UFSFastConstructMenu::InitItem()
{
	this->WidgetWidth = 220.0f;

	AddChild(Cast<UFSMenuItem>(NewObject<UFSFoundationIConstructMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSFoundationIIConstructMenu>(this)));
}

UFSMenuEntryBase* UFSNewDesignMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("New Design")));
	return Entry;
}

void UFSNewDesignMenu::ExecClick()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	UFSDesignMenu* Design = SkylineUIVar->FindLastDesignItem();
	UFSDesignMenu* Child = UFSDesignMenu::CreateDesignMenu(Design, true);
	Design->AddChild(Child);
	if (SkylineUIVar->Turtioul) {
		SkylineUIVar->UnexpandStart(false);
		SkylineUIVar->FinishTurtioul();
	}
	else {
		SkylineUIVar->UnexpandStart(true);
		Child->FSClick();
	}
}


UFSMenuEntryBase* UFSNewFolderMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("New Folder")));
	return Entry;
}

void UFSNewFolderMenu::ExecClick()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	UFSDesignMenu* Design = SkylineUIVar->FindLastDesignItem();
	UFSDesignMenu* Child = UFSDesignMenu::CreateDesignMenu(Design, false);
	Child->FileName = FText::FromString(TEXT("Unnamed Folder"));
	Design->AddChild(Child);
	if (SkylineUIVar->Turtioul) {
		SkylineUIVar->UnexpandStart(false);
		SkylineUIVar->FinishTurtioul();
	}
	else {
		SkylineUIVar->UnexpandStart(true);
		Child->FSClick();
	}
}

UFSMenuEntryBase* UFSKeySettingMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Key Settings")));
	return Entry;
}

void UFSKeySettingMenu::ExecClick()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	if (SkylineUIVar->Turtioul) {
		SkylineUIVar->UnexpandStart(false);
	}
	else {
		SkylineUIVar->UnexpandStart(true);
	}
	SkylineUIVar->KeySettingsWidget->Start();
}

UFSMenuEntryBase* UFSInventoryMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Inventory")));
	return Entry;
}

void UFSInventoryMenu::ExecClick()
{
	UFSkylineUI* SkylineUIVar = Cast<UFSkylineUI>(this->SkylineUI);
	if (SkylineUIVar->Turtioul) {
		SkylineUIVar->UnexpandStart(false);
	}
	else {
		SkylineUIVar->UnexpandStart(true);
	}
	SkylineUIVar->InventoryWidget->Toggle();
}

UFSMenuEntryBase* UFSTookitMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Tookits")));
	return Entry;
}

void UFSTookitMenu::InitItem()
{
	this->WidgetWidth = 300.0f;

	AddChild(Cast<UFSMenuItem>(NewObject<UFSInventoryOptionsMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSTookitFlyMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSFogControlMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSGodlikeMenu>(this)));
	//AddChild(Cast<UFSMenuItem>(NewObject<UFSThirdPersonViewMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSInfiniteAmmoMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSDontSaveAnythingMenu>(this)));
}

UFSMenuEntryBase* UFSFoundationIConstructMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Foundation I")));
	return Entry;
}

UFSMenuEntryBase* UFSFoundationIIConstructMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Foundation II")));
	return Entry;
}

UFSMenuEntryBase* UFSInventoryOptionsMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Inventory Options")));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Entry->HorBox->Slot);
	Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Slot->SetOffsets(FMargin(40.0f, 0.0f, 0.0f, 0.0f));
	return Entry;
}

void UFSInventoryOptionsMenu::InitItem()
{
	this->WidgetWidth = 500.0f;

	AddChild(Cast<UFSMenuItem>(NewObject<UFSRecycleMaterialsMenu>(this)));
}

void UFSRecycleMaterialsMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Recycle Materials From AWESOME Sink")));
}

void UFSRecycleMaterialsMenu::InitItem()
{
	this->AttributeName = "RecycleMaterials";
	Super::InitItem();
}

void UFSRecycleMaterialsMenu::onSwitcherChanged()
{
	AFSkyline::Get(this)->FSCtrl->onSwitchRecycleMaterials();
}

UFSMenuEntryBase* UFSTookitFlyMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("I Wanna Fly")));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Entry->HorBox->Slot);
	Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Slot->SetOffsets(FMargin(40.0f, 0.0f, 0.0f, 0.0f));
	return Entry;
}

void UFSTookitFlyMenu::InitItem()
{
	this->WidgetWidth = 300.0f;

	AddChild(Cast<UFSMenuItem>(NewObject<UFSDoubleSpaceFlyingMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSDoubleShiftSprintMenu>(this)));
}

void UFSDoubleSpaceFlyingMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Switch Flying")));
}

void UFSDoubleSpaceFlyingMenu::InitItem()
{
	this->AttributeName = "DoubleSpaceFlying";
	Super::InitItem();
}

void UFSDoubleSpaceFlyingMenu::onSwitcherChanged()
{
	AFSkyline::Get(this)->FSCtrl->onSwitchFlying();
}

void UFSDoubleShiftSprintMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Launch Turbo Sprint")));
}

void UFSDoubleShiftSprintMenu::InitItem()
{
	this->AttributeName = "DoubleShiftSprint";
	Super::InitItem();
}

UFSMenuEntryBase* UFSFogControlMenu::ConstructEntry()
{
	UFSButtonWidget* Entry = CreateUserWidget<UFSButtonWidget>(TEXT("/FactorySkyline/Widget_FSButton.Widget_FSButton_C"));
	if (!Entry) return nullptr;
	Entry->MenuText->SetText(FText::FromString(TEXT("Fog Control")));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Entry->HorBox->Slot);
	Slot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
	Slot->SetOffsets(FMargin(40.0f, 0.0f, 0.0f, 0.0f));
	return Entry;
}

void UFSFogControlMenu::InitItem()
{
	this->WidgetWidth = 300.0f;

	AddChild(Cast<UFSMenuItem>(NewObject<UFSDisableFogMenu>(this)));
	AddChild(Cast<UFSMenuItem>(NewObject<UFSDisableHighSpaceFogMenu>(this)));
}

void UFSDisableFogMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Disable Fog")));
}

void UFSDisableFogMenu::InitItem()
{
	this->AttributeName = "DisableFog";
	Super::InitItem();
}

void UFSDisableFogMenu::onSwitcherChanged()
{
	AFSkyline::Get(this)->FSCtrl->onSwitchFog();
}

void UFSDisableHighSpaceFogMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Disable High Space Fog")));
}

void UFSDisableHighSpaceFogMenu::InitItem()
{
	this->AttributeName = "DisableHighSpaceFog";
	Super::InitItem();
}

void UFSGodlikeMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Beyond Godlike")));
}

void UFSGodlikeMenu::InitItem()
{
	this->AttributeName = "Godlike";
	Super::InitItem();
}

void UFSGodlikeMenu::onSwitcherChanged()
{
	AFSkyline::Get(this)->FSCtrl->onSwitchGodlike();
}

void UFSThirdPersonViewMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Third Person View")));
}

void UFSThirdPersonViewMenu::InitItem()
{
	this->AttributeName = "ThirdPersonView";
	Super::InitItem();
}

void UFSThirdPersonViewMenu::onSwitcherChanged()
{
	AFSkyline::Get(this)->FSCtrl->onSwitchThirdPersonView();
}

void UFSInfiniteAmmoMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Infinite Ammo")));
}

void UFSInfiniteAmmoMenu::InitItem()
{
	this->AttributeName = "InfiniteAmmo";
	Super::InitItem();
}

void UFSDontSaveAnythingMenu::InitEntry(UFSButtonWidget* Entry)
{
	Entry->MenuText->SetText(FText::FromString(TEXT("Don't Save Anything")));
}

void UFSDontSaveAnythingMenu::InitItem()
{
	this->AttributeName = "DontSaveAnything";
	Super::InitItem();
}
