// ILikeBanas


#include "FactorySkyline/UI/FSkylineUI.h"
#include "FactorySkyline/UI/FSListMenuWidget.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSMenuItem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "FactorySkyline/FSDesign.h"
#include "FactorySkyline/UI/FSKeyMappingWidget.h"



UFSkylineUI::UFSkylineUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

FReply UFSkylineUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UFSkylineUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UFSkylineUI::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UFSkylineUI::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//UFSButtonWidget* Entry = Cast<UFSButtonWidget>(GettingStart->AccquireEntry());
	//Entry->MenuText->SetText(FText::FromString(FString::FromInt(int(rand()))));
	this->FSMouseMove(InMouseEvent);
	return FReply::Handled();
}

void UFSkylineUI::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	this->IsMouseCursorOnUI = true;
}

void UFSkylineUI::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	this->IsMouseCursorOnUI = false;
}

void UFSkylineUI::FSMouseDown(UFSButtonWidget* ButtonWidget, const FPointerEvent& InMouseEvent)
{
	LastButtonMouseDown = ButtonWidget;
	LastKeyMouseDown = InMouseEvent.GetEffectingButton();
}

bool UFSkylineUI::FSMouseUp(UFSButtonWidget* ButtonWidget, const FPointerEvent& InMouseEvent)
{
	if (LastButtonMouseDown == ButtonWidget && LastKeyMouseDown == InMouseEvent.GetEffectingButton()) {
		LastButtonMouseDown = nullptr;
		return true;
	}
	LastButtonMouseDown = nullptr;
	return false;
}

void UFSkylineUI::FSMouseMove(const FPointerEvent& InMouseEvent)
{
}

void UFSkylineUI::FSMouseMove(UFSButtonWidget* ButtonWidget, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
		!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		ResetHighLight();
		if (ButtonWidget && ButtonWidget->IsEnabled) {
			CurrentHighLightButton = ButtonWidget;
			ButtonWidget->SetHightLightView();
		}
	}
	FSMouseMove(InMouseEvent);
}

void UFSkylineUI::FSMouseMove(UFSMenuWidget* MenuWidget, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
		!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		ResetHighLight();
	}
	FSMouseMove(InMouseEvent);
}

void UFSkylineUI::FSMouseMove(UFSInventoryWidget* MenuWidget, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
		!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		ResetHighLight();
	}
	FSMouseMove(InMouseEvent);
}

void UFSkylineUI::FSMouseLeave(UFSMenuWidget* MenuWidget, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
		!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		ResetHighLight();
	}
}

void UFSkylineUI::FSMouseLeave(UFSInventoryWidget* MenuWidget, const FPointerEvent& InMouseEvent)
{
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) &&
		!InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton)) {
		ResetHighLight();
	}
}


bool UFSkylineUI::Init(UWorld* World)
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	DesignRoot = Skyline->FSCtrl->DesignRoot;
	//Turtioul = true;

	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_FSLogo.Widget_FSLogo_C"));
	if (!WidgetClass) return false;
	Logo = Cast<UFSLogoWidget>(CreateWidget<UUserWidget>(this, WidgetClass));
	if (!Logo) return false;

	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_FSOperating.Widget_FSOperating_C"));
	if (!WidgetClass) return false;
	OperatingWidget = CreateWidget<UFSOperatingWidget>(this, WidgetClass);
	if (!OperatingWidget) return false;

	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_Completion.Widget_Completion_C"));
	if (!WidgetClass) return false;
	CompletionWidget = CreateWidget<UFSCompletionWidget>(this, WidgetClass);
	if (!CompletionWidget) return false;
	
	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_RepeatConsole.Widget_RepeatConsole_C"));
	if (!WidgetClass) return false;
	RepeatWidget = CreateWidget<UFSRepeatWidget>(this, WidgetClass);
	if (!RepeatWidget) return false;

	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_RectSelection.Widget_RectSelection_C"));
	if (!WidgetClass) return false;
	SelectRect = CreateWidget<UFSSelectRect>(this, WidgetClass);
	if (!SelectRect) return false;

	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_KeySettings.Widget_KeySettings_C"));
	if (!WidgetClass) return false;
	KeySettingsWidget = CreateWidget<UFSKeySettingsWidget>(this, WidgetClass);
	if (!KeySettingsWidget) return false;

	WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_InventoryPanel.Widget_InventoryPanel_C"));
	if (!WidgetClass) return false;
	InventoryWidget = CreateWidget<UFSInventoryWidget>(this, WidgetClass);
	if (!InventoryWidget) return false;

	PressCtrlMapping = AddKeyMappingHelper(FText::FromString(TEXT("Show Mouse Cursor")), FText::FromString(TEXT("Short Press Left Ctrl")));
	HoldCtrlMapping = AddKeyMappingHelper(FText::FromString(TEXT("Area Select / Unselect")), FText::FromString(TEXT("Hold Left Ctrl + LMB / RMB")));
	SelectScrollMapping = AddKeyMappingHelper(FText::FromString(TEXT("Select One")), FText::FromString(TEXT("Scroll Up / Scroll Down")));
	SelectMapping = AddKeyMappingHelper(FText::FromString(TEXT("Set Selected / Nagetive")), FText::FromString(TEXT("LMB")));
	SelectAnchorMapping = AddKeyMappingHelper(FText::FromString(TEXT("Select / Unset Anchor")), FText::FromString(TEXT("LMB + RMB")));
	GotoBuildMapping = AddKeyMappingHelper(FText::FromString(TEXT("Goto Build")), FText::FromString(TEXT("Left Ctrl + R")));
	CopyScrollMapping = AddKeyMappingHelper(FText::FromString(TEXT("Rotate")), FText::FromString(TEXT("Scroll Up / Scroll Down")));
	CopyMapping = AddKeyMappingHelper(FText::FromString(TEXT("Build")), FText::FromString(TEXT("LMB")));
	SetRepeatMapping = AddKeyMappingHelper(FText::FromString(TEXT("Set Repeat Location")), FText::FromString(TEXT("LMB")));
	SetAnchorMapping = AddKeyMappingHelper(FText::FromString(TEXT("Set Anchor")), FText::FromString(TEXT("LMB")));
	SetCircleCenterMapping = AddKeyMappingHelper(FText::FromString(TEXT("Set Circle Center")), FText::FromString(TEXT("LMB")));
	BackToSelectMapping = AddKeyMappingHelper(FText::FromString(TEXT("Back To Select")), FText::FromString(TEXT("Left Ctrl + R")));
	ExitMapping = AddKeyMappingHelper(FText::FromString(TEXT("Close")), FText::FromString(TEXT("R / RMB")));
	
	GettingStart = NewObject<UFSGettingStartMenu>(this);
	GettingStart->SkylineUI = this;
	GettingStart->InitItem();

	LogoSlot = RootPanel->AddChildToCanvas(Logo);
	if (!Logo->Init(GettingStart)) return false;

	OperatingWidget->Init();
	OperatingWidget->SkylineUI = this;

	RepeatWidget->SetVisibility(ESlateVisibility::Collapsed);
	UCanvasPanelSlot* Slot = RepeatBox->AddChildToCanvas(RepeatWidget);
	Slot->SetAnchors(FAnchors(0.5f, 0.0f));
	Slot->SetPosition(FVector2D(-150.0f, 0.0f));
	RepeatWidget->Init();

	CompletionWidget->SetVisibility(ESlateVisibility::Collapsed);
	RootPanel->AddChildToCanvas(CompletionWidget);
	CompletionWidget->Init();

	this->SetVisibility(ESlateVisibility::Hidden);
	this->IsActive = false;
	this->AddToViewport();

	SelectRect->Init();
	SelectRect->SetVisibility(ESlateVisibility::Collapsed);
	SelectRect->AddToViewport(1);

	KeySettingsWidget->Init();
	KeySettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
	KeySettingsWidget->AddToViewport(1);

	InventoryWidget->Init(&Skyline->FSCtrl->Inventory);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	InventoryWidget->AddToViewport(0);
	
	this->IsMouseCursorOnUI = false;
	if (DesignRoot) this->ShowDesign();

	return true;
}


void UFSkylineUI::OpenMenu()
{
	if (IsActive) return;

	LastButtonMouseDown = nullptr;
	ResetHighLight();
	//if (!Turtioul) ExpandDesign();

	if (Turtioul) {
		Logo->ResetLogo();
		Logo->StartEnterAnimation();
	}
	else {
		Logo->ShowLogo();
		Logo->Time.Start();
	}
	this->IsActive = true;
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UFSkylineUI::HideMenu()
{
	if (!IsActive) return;
	GettingStart->Unexpand();
	//DesignRoot->Unexpand();
	this->SetVisibility(ESlateVisibility::Hidden);
	this->IsActive = false;

	this->CompletionWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UFSkylineUI::FinishTurtioul()
{
	if (Turtioul) {
		Turtioul = false;
		ShowDesign();
		Logo->StartFoldAnimation();
	}
}

void UFSkylineUI::ShowDesign()
{
	ResetHighLight();
	DesignRoot->Expand(FVector2D(0.0, 318.0));
}

void UFSkylineUI::FoldDesign()
{
	UFSDesignMenu* DesignMenu = FindLastDesignItem();
	if (DesignMenu->CurrentExpandedItem) DesignMenu = Cast<UFSDesignMenu>(DesignMenu->CurrentExpandedItem);
	if (DesignMenu == DesignRoot) return;
	UFSDesignMenu* Parent = Cast<UFSDesignMenu>(DesignMenu->ParentItem);
	if (Parent && !Parent->Folded) DesignMenu->Fold();
}

void UFSkylineUI::FoldCurrent()
{
	UFSDesignMenu* Item = Cast<UFSDesignMenu>(OperatingWidget->ParentItem);
	if (Item) {
		UFSDesignMenu* Parent = Cast<UFSDesignMenu>(Item->ParentItem);
		if (Parent && !Parent->Folded) Item->Fold();
	}
}

void UFSkylineUI::ExpandStart()
{
	ResetHighLight();
	if (!Turtioul) {
		LastFold = FindLastDesignItem();
		if (LastFold->CurrentExpandedItem) LastFold = Cast<UFSDesignMenu>(LastFold->CurrentExpandedItem);
		if (LastFold == DesignRoot) LastFold = nullptr;
		else {
			UFSDesignMenu* Parent = Cast<UFSDesignMenu>(LastFold->ParentItem);
			if (!Parent->Folded) LastFold->Fold();
			else  LastFold = nullptr;
		}
	}
	Logo->ExpandStart();
}

void UFSkylineUI::UnexpandStart(bool RestoreDesign)
{
	this->GettingStart->Unexpand();
	this->Logo->Time.Start();
	if (!Turtioul && LastFold) {
		if (RestoreDesign) LastFold->Unfold();
		LastFold = nullptr;
	}
}

void UFSkylineUI::SetHighLightMapping(UFSKeyMappingWidget* Mapping)
{
	RestoreHighLightMapping();
	Mapping->SetHighLightView();
	CurrentHighLightMapping = Mapping;
}

void UFSkylineUI::RestoreHighLightMapping()
{
	if (!CurrentHighLightMapping) return;
	CurrentHighLightMapping->SetDefaultView();
	CurrentHighLightMapping = nullptr;
}

void UFSkylineUI::ResetHighLight()
{
	if (CurrentHighLightButton) {
		CurrentHighLightButton->UpdateView();
		CurrentHighLightButton = nullptr;
	}
}

UFSKeyMappingWidget* UFSkylineUI::AddKeyMappingHelper(const FText& Title, const FText& Key)
{
	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_KeyMapping.Widget_KeyMapping_C"));
	if (!WidgetClass) return nullptr;
	UFSKeyMappingWidget* KeyMappingWidget = Cast<UFSKeyMappingWidget>(CreateWidget<UFSKeyMappingWidget>(this, WidgetClass));
	if (!KeyMappingWidget) return nullptr;
	
	KeyMappingWidget->SetVisibility(ESlateVisibility::Collapsed);
	KeyMappingWidget->SetTitle(Title);
	KeyMappingWidget->SetKey(Key);
	OperatingHelper->AddChild(KeyMappingWidget);
	KeyMappingWidget->SetPadding(10.0f);

	return KeyMappingWidget;
}

UFSDesignMenu* UFSkylineUI::FindLastDesignItem()
{
	UFSDesignMenu* Design = DesignRoot;
	while (UFSDesignMenu* Next = Cast<UFSDesignMenu>(Design->CurrentExpandedItem)) {
		if (Next->Design) break;
		Design = Next;
	}
	return Design;
}