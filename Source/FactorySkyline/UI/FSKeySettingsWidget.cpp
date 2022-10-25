// ILikeBanas


#include "FSKeySettingsWidget.h"
#include "FactorySkyline/FSkyline.h"


UFSKeySettingsWidget::UFSKeySettingsWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSKeySettingsWidget::onExit()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	this->SetVisibility(ESlateVisibility::Collapsed);

	FSkyline->FSCtrl->FSInput->IsActive = true;
	FSkyline->FSCtrl->FSInput->IsScaning = false;
	FSkyline->FSCtrl->FSInput->Scaner.CurrentEntry = nullptr;
}

void UFSKeySettingsWidget::onReset()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	FSkyline->FSCtrl->FSInput->UpdatePreSet();
	FSkyline->FSCtrl->FSInput->ApplySetting();
}

void UFSKeySettingsWidget::onApply()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	FSkyline->FSCtrl->FSInput->ApplySetting();
}

void UFSKeySettingsWidget::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;
	
	FSkyline->FSCtrl->FSInput->InitWidget();

	FScriptDelegate Func;
	Func.BindUFunction(this, FName("onExit"));
	this->ExitButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onReset"));
	this->ResetButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onApply"));
	this->ApplyButton->OnClicked.Add(Func);
}

void UFSKeySettingsWidget::Start()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	this->SetVisibility(ESlateVisibility::Visible);

	FSkyline->FSCtrl->FSInput->UpdateCurrent();
	FSkyline->FSCtrl->FSInput->IsActive = false;
	FSkyline->FSCtrl->FSInput->IsScaning = true;
	FSkyline->FSCtrl->FSInput->Scaner.CurrentEntry = nullptr;
}

void UFSKeySettingsWidget::AddEntry(const FName& Action)
{
	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_KeySettingPanel.Widget_KeySettingPanel_C"));
	if (!WidgetClass) return;
	UFSKeySettingEntry* Entry = CreateWidget<UFSKeySettingEntry>(this, WidgetClass);
	if (!Entry) return;
	Entry->Init();
	Entry->Action = Action;
	Box->AddChild(Entry);
	EntryMapping.Add(Action, Entry);
}

void UFSKeySettingsWidget::UpdateKey(const FName& Action, const FSKey& Key, const FText& Desc)
{
	UFSKeySettingEntry** Entry = EntryMapping.Find(Action);
	if (Entry) {
		(*Entry)->KeyString = Key.ToString();
		(*Entry)->Title->SetText(Desc);
		(*Entry)->KeyDesc->SetText(FText::FromString(Key.GetDesc()));
	}
}

FReply UFSKeySettingsWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UFSKeySettingsWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}