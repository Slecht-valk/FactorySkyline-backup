// ILikeBanas


#include "FSInput.h"
#include "Components/EditableText.h"
#include "FSController.h"
#include "FSkyline.h"
#include "UI/FSKeySettingsWidget.h"
#include "UI/FSKeySettingEntry.h"


void UFSInput::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	this->FGController = FSkyline->FGController;

	this->InitKey("MenuKey", FSKey(EKeys::R), FText::FromString(TEXT("Call Menu")), true);
	this->InitKey("EscapeKey", FSKey(EKeys::Escape), FText::FromString(TEXT("Close Menu")), false);
	this->InitKey("InventoryKey", FSKey(EKeys::I), FText::FromString(TEXT("Open / Close Inventory")), true);
	this->InitKey("LeftControlKey", FSKey(EKeys::LeftControl), FText::FromString(TEXT("Left Control")), true);
	this->InitKey("FlyingKey", FSKey(EKeys::SpaceBar, false, false, false, true), FText::FromString(TEXT("Toggle Flying")), true);
	this->InitKey("SprintKey", FSKey(EKeys::LeftShift), FText::FromString(TEXT("Sprint")), true);
	this->InitKey("TurboSprintKey", FSKey(EKeys::LeftShift, false, false, false, true), FText::FromString(TEXT("Turbo Sprint")), true);
	this->InitKey("ScrollUpKey", FSKey(EKeys::MouseScrollUp), FText::FromString(TEXT("Mouse Scroll Up")), true);
	this->InitKey("ScrollDownKey", FSKey(EKeys::MouseScrollDown), FText::FromString(TEXT("Mouse Scroll Down")), true);
	this->InitKey("LeftMouseKey", FSKey(EKeys::LeftMouseButton), FText::FromString(TEXT("Left Mouse Button")), false);
	this->InitKey("RightMouseKey", FSKey(EKeys::RightMouseButton), FText::FromString(TEXT("Right Mouse Button")), false);
	this->InitKey("ActionShortcut1Key", FSKey(EKeys::R, true), FText::FromString(TEXT("Action Shortcut")), true);
	this->InitKey("ForwardKey", FSKey(EKeys::W), FText::FromString(TEXT("Move Forward")), true);
	this->InitKey("BackwardKey", FSKey(EKeys::S), FText::FromString(TEXT("Move Backward")), true);
	this->InitKey("LeftKey", FSKey(EKeys::A), FText::FromString(TEXT("Move Left")), true);
	this->InitKey("RightKey", FSKey(EKeys::D), FText::FromString(TEXT("Move Right")), true);
	this->InitKey("GoUpKey", FSKey(EKeys::SpaceBar), FText::FromString(TEXT("Move Up")), true);
	this->InitKey("GoDownKey", FSKey(EKeys::C), FText::FromString(TEXT("Move Down")), true);
	this->InitKey("DeleteKey", FSKey(EKeys::Delete, true), FText::FromString(TEXT("Delete the Last Opening Folder")), true);

	Scaner.Init(FGController);
	IsActive = true;
}

void UFSInput::Tick()
{
	bool Ctrl = FGController->IsInputKeyDown(EKeys::LeftControl) || FGController->IsInputKeyDown(EKeys::RightControl);
	bool Alt = FGController->IsInputKeyDown(EKeys::LeftAlt) || FGController->IsInputKeyDown(EKeys::RightAlt);
	bool Shift = FGController->IsInputKeyDown(EKeys::LeftShift) || FGController->IsInputKeyDown(EKeys::RightShift);
	for (TPair<FName, FSActionKey>& Pair : KeyMapping) {
		FSActionKey& ActionKey = Pair.Value;
		FSKey& Key = ActionKey.Key;
		ActionKey.KeyDown = (IsActive || Key.Key == EKeys::Escape) && FGController->IsInputKeyDown(Key.Key);
	}
	for (TPair<FName, FSActionKey>& Pair : KeyMapping) {
		FSActionKey& ActionKey = Pair.Value;
		FSKey& Key = ActionKey.Key;
		bool CheckCtrl = Key.Ctrl == Ctrl || Key.Key == EKeys::LeftControl || Key.Key == EKeys::RightControl;
		bool CheckAlt = Key.Alt == Alt || Key.Key == EKeys::LeftAlt || Key.Key == EKeys::RightAlt;
		bool CheckShift = Key.Shift == Shift || Key.Key == EKeys::LeftShift || Key.Key == EKeys::RightShift;
		bool Check = CheckCtrl && CheckAlt && CheckShift && (IsActive || Key.Key == EKeys::Escape);
		if (ActionKey.KeyDown) {
			if (ActionKey.LastDown) {
				if (Check) ActionKey.OnKeyRepeat.ExecuteIfBound();
			}
			else {
				if (Check) {
					if (Key.Double) {
						if (ActionKey.LastPress.GetTime() < DoubleClickTime) {
							ActionKey.OnKeyPressed.ExecuteIfBound();
							ActionKey.LastPress.Start(-10);
						}
						else ActionKey.LastPress.Start();
					}
					else {
						ActionKey.OnKeyPressed.ExecuteIfBound();
						ActionKey.LastPress.Start();
					}
					ActionKey.LastDown = true;
				}
			}
		}
		else {
			if (ActionKey.LastDown) {
				ActionKey.OnKeyReleased.ExecuteIfBound();
				ActionKey.LastDown = false;
			}
			else {
				if (Check && FGController->WasInputKeyJustPressed(Key.Key)) {
					if (Key.Double) {
						if (ActionKey.LastPress.GetTime() < DoubleClickTime) {
							ActionKey.OnKeyPressed.ExecuteIfBound();
							ActionKey.LastPress.Start(-10);
						}
						else ActionKey.LastPress.Start();
					}
					else {
						ActionKey.OnKeyPressed.ExecuteIfBound();
						ActionKey.OnKeyReleased.ExecuteIfBound();
						ActionKey.LastPress.Start();
					}
				}
			}
		}
	}

	if (IsScaning) {
		AFSkyline* FSkyline = AFSkyline::Get(this);
		if (!FSkyline) return;

		if (FSkyline->SkylineUI->KeySettingsWidget->GetVisibility() != ESlateVisibility::Visible) {
			IsScaning = false;
			IsActive = true;
		}
		else {
			if (!FSkyline->FSCtrl->IsShowMouseCursor()) FSkyline->FSCtrl->ShowMouseCursor();
			Scaner.Tick();
		}
	}
}

void UFSInput::InitKey(const FName& Action, const FSKey& Key, const FText& Desc, bool Configable)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	FSActionKey* ActionKey = nullptr;

	if (FSkyline->FSCtrl->Etc->HasKey(Action)) {
		ActionKey = BindKey(Action, FSKey::FromString(FSkyline->FSCtrl->Etc->Get(Action)));
	} else ActionKey = BindKey(Action, Key);

	ActionKey->Desc = Desc;
	ActionKey->PreSet = Key;
	ActionKey->Configable = Configable;
	if (Configable) ConfigList.Add(Action);
}

FSActionKey* UFSInput::BindKey(const FName& Action, const FSKey& Key)
{
	FSActionKey* ActionKey = KeyMapping.Find(Action);
	if (!ActionKey) {
		KeyMapping.Add(Action, FSActionKey(Action, Key));
		ActionKey = KeyMapping.Find(Action);
	}
	ActionKey->Key = Key;

	AFSkyline* FSkyline = AFSkyline::Get(this);
	FSkyline->FSCtrl->Etc->Set(Action, Key.ToString());

	return ActionKey;
}

bool UFSInput::IsKeyDown(const FName& Action)
{
	FSActionKey* ActionKey = KeyMapping.Find(Action);
	if (ActionKey) return ActionKey->KeyDown;
	return false;
}


void UFSInput::InitWidget()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	UFSKeySettingsWidget* Setting = FSkyline->SkylineUI->KeySettingsWidget;
	for (FName& Action : ConfigList) {
		Setting->AddEntry(Action);
	}
}

void UFSInput::UpdateCurrent()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	UFSKeySettingsWidget* Setting = FSkyline->SkylineUI->KeySettingsWidget;
	for (FName& Action : ConfigList) {
		FSActionKey* ActionKey = KeyMapping.Find(Action);
		if (ActionKey) Setting->UpdateKey(Action, ActionKey->Key, ActionKey->Desc);
	}
}

void UFSInput::UpdatePreSet()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	UFSKeySettingsWidget* Setting = FSkyline->SkylineUI->KeySettingsWidget;
	for (FName& Action : ConfigList) {
		FSActionKey* ActionKey = KeyMapping.Find(Action);
		if (ActionKey) Setting->UpdateKey(Action, ActionKey->PreSet, ActionKey->Desc);
	}
}

void UFSInput::ApplySetting()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	UFSKeySettingsWidget* Setting = FSkyline->SkylineUI->KeySettingsWidget;
	for (FName& Action : ConfigList) {
		UFSKeySettingEntry** Entry = Setting->EntryMapping.Find(Action);
		if (Entry) BindKey(Action, FSKey::FromString((*Entry)->KeyString));
	}
}



void FScaner::Init(AFGPlayerController* FGControllerParam)
{
	this->FGController = FGControllerParam;

	AddKey(EKeys::Escape);

	AddKey(EKeys::MouseScrollUp);
	AddKey(EKeys::MouseScrollDown);

	AddKey(EKeys::MiddleMouseButton);
	AddKey(EKeys::ThumbMouseButton);
	AddKey(EKeys::ThumbMouseButton2);

	AddKey(EKeys::BackSpace);
	AddKey(EKeys::Tab);
	AddKey(EKeys::Enter);
	AddKey(EKeys::Pause);

	AddKey(EKeys::CapsLock);
	AddKey(EKeys::SpaceBar);
	AddKey(EKeys::PageUp);
	AddKey(EKeys::PageDown);
	AddKey(EKeys::End);
	AddKey(EKeys::Home);

	AddKey(EKeys::Left);
	AddKey(EKeys::Up);
	AddKey(EKeys::Right);
	AddKey(EKeys::Down);

	AddKey(EKeys::Insert);
	AddKey(EKeys::Delete);

	AddKey(EKeys::Zero);
	AddKey(EKeys::One);
	AddKey(EKeys::Two);
	AddKey(EKeys::Three);
	AddKey(EKeys::Four);
	AddKey(EKeys::Five);
	AddKey(EKeys::Six);
	AddKey(EKeys::Seven);
	AddKey(EKeys::Eight);
	AddKey(EKeys::Nine);

	AddKey(EKeys::A);
	AddKey(EKeys::B);
	AddKey(EKeys::C);
	AddKey(EKeys::D);
	AddKey(EKeys::E);
	AddKey(EKeys::F);
	AddKey(EKeys::G);
	AddKey(EKeys::H);
	AddKey(EKeys::I);
	AddKey(EKeys::J);
	AddKey(EKeys::K);
	AddKey(EKeys::L);
	AddKey(EKeys::M);
	AddKey(EKeys::N);
	AddKey(EKeys::O);
	AddKey(EKeys::P);
	AddKey(EKeys::Q);
	AddKey(EKeys::R);
	AddKey(EKeys::S);
	AddKey(EKeys::T);
	AddKey(EKeys::U);
	AddKey(EKeys::V);
	AddKey(EKeys::W);
	AddKey(EKeys::X);
	AddKey(EKeys::Y);
	AddKey(EKeys::Z);

	AddKey(EKeys::NumPadZero);
	AddKey(EKeys::NumPadOne);
	AddKey(EKeys::NumPadTwo);
	AddKey(EKeys::NumPadThree);
	AddKey(EKeys::NumPadFour);
	AddKey(EKeys::NumPadFive);
	AddKey(EKeys::NumPadSix);
	AddKey(EKeys::NumPadSeven);
	AddKey(EKeys::NumPadEight);
	AddKey(EKeys::NumPadNine);

	AddKey(EKeys::Multiply);
	AddKey(EKeys::Add);
	AddKey(EKeys::Subtract);
	AddKey(EKeys::Decimal);
	AddKey(EKeys::Divide);

	AddKey(EKeys::F1);
	AddKey(EKeys::F2);
	AddKey(EKeys::F3);
	AddKey(EKeys::F4);
	AddKey(EKeys::F5);
	AddKey(EKeys::F6);
	AddKey(EKeys::F7);
	AddKey(EKeys::F8);
	AddKey(EKeys::F9);
	AddKey(EKeys::F10);
	AddKey(EKeys::F11);
	AddKey(EKeys::F12);

	AddKey(EKeys::NumLock);

	AddKey(EKeys::ScrollLock);

	AddKey(EKeys::LeftShift);
	AddKey(EKeys::RightShift);
	AddKey(EKeys::LeftControl);
	AddKey(EKeys::RightControl);
	AddKey(EKeys::LeftAlt);
	AddKey(EKeys::RightAlt);
	AddKey(EKeys::LeftCommand);
	AddKey(EKeys::RightCommand);

	AddKey(EKeys::Semicolon);
	AddKey(EKeys::Equals);
	AddKey(EKeys::Comma);
	AddKey(EKeys::Underscore);
	AddKey(EKeys::Hyphen);
	AddKey(EKeys::Period);
	AddKey(EKeys::Slash);
	AddKey(EKeys::Tilde);
	AddKey(EKeys::LeftBracket);
	AddKey(EKeys::Backslash);
	AddKey(EKeys::RightBracket);
	AddKey(EKeys::Apostrophe);

	AddKey(EKeys::Ampersand);
	AddKey(EKeys::Asterix);
	AddKey(EKeys::Caret);
	AddKey(EKeys::Colon);
	AddKey(EKeys::Dollar);
	AddKey(EKeys::Exclamation);
	AddKey(EKeys::LeftParantheses);
	AddKey(EKeys::RightParantheses);
	AddKey(EKeys::Quote);
}

void FScaner::Load(UFSKeySettingEntry* Entry)
{
	this->CurrentEntry = Entry;
}

void FScaner::Unload(UFSKeySettingEntry* Entry)
{
	if (this->CurrentEntry == Entry)
		this->CurrentEntry = nullptr;
}

void FScaner::Tick()
{
	bool Ctrl = FGController->IsInputKeyDown(EKeys::LeftControl) || FGController->IsInputKeyDown(EKeys::RightControl);
	bool Alt = FGController->IsInputKeyDown(EKeys::LeftAlt) || FGController->IsInputKeyDown(EKeys::RightAlt);
	bool Shift = FGController->IsInputKeyDown(EKeys::LeftShift) || FGController->IsInputKeyDown(EKeys::RightShift);
	for (FSActionKey& ActionKey : ScaningList) {
		FKey& Key = ActionKey.Key.Key;
		if (FGController->IsInputKeyDown(Key)) {
			if (!ActionKey.LastDown) {
				bool Double = ActionKey.LastPress.GetTime() < DoubleClickTime;
				Update(FSKey(Key, Ctrl, Alt, Shift, Double));
				if (Double) ActionKey.LastPress.Start(-10);
				else ActionKey.LastPress.Start();
				ActionKey.LastDown = true;
			}
		}
		else {
			if (FGController->WasInputKeyJustPressed(Key)) {
				bool Double = ActionKey.LastPress.GetTime() < DoubleClickTime;
				Update(FSKey(Key, Ctrl, Alt, Shift, Double));
				if (Double) ActionKey.LastPress.Start(-10);
				else ActionKey.LastPress.Start();
			}
			ActionKey.LastDown = false;
		}
	}
}

void FScaner::AddKey(const FKey& Key)
{
	ScaningList.Add(FSActionKey("", FSKey(Key)));
}

void FScaner::Update(const FSKey& Key)
{
	if (Key.Key == EKeys::Escape) {
		AFSkyline* FSkyline = AFSkyline::Get(FGController);
		if (!FSkyline) return;
		FSkyline->SkylineUI->KeySettingsWidget->onExit();
		return;
	}
	if (!CurrentEntry) return;
	CurrentEntry->KeyString = Key.ToString();
	CurrentEntry->KeyDesc->SetText(FText::FromString(Key.GetDesc()));
}
