// ILikeBanas


#include "FSController.h"
#include "FSkyline.h"
#include "FSBuilder.h"
#include "FSDesign.h"
#include "FSSelection.h"
#include "UI/FSkylineUI.h"
#include "FGPlayerController.h"
#include "FGCharacterPlayer.h"
#include "FGHealthComponent.h"
#include "UI/FGVirtualCursorFunctionLibrary.h"
#include "UI/FGGameUI.h"
#include "Equipment/FGBuildGun.h"
#include "Equipment/FGWeapon.h"
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include <string>


void AFSController::Init() {
	AFSkyline* FSkyline = AFSkyline::Get(this);

	this->World = FSkyline->World;
	this->WorldSettings = FSkyline->WorldSettings;
	this->FGController = FSkyline->FGController;
	this->ResourceSinkSubsystem = AFGResourceSinkSubsystem::Get(FSkyline->World);
	this->FGUI = FSkyline->GameUI;
	this->HUD = FSkyline->HUD;
	GetPlayer();

	this->SkylineUI = FSkyline->SkylineUI;
	this->Builder = FSkyline->Builder;
	this->Select = FSkyline->Select;

	if (!this->Etc) this->Etc = NewObject<UFSEtc>(this);
	this->FSInput = NewObject<UFSInput>(this);

	this->Etc->Init();
	this->FSInput->Init();
	this->Inventory.Init(FSkyline->SplineHologramFactory);

	if (this->DesignRoot) {
		SkylineUI->Turtioul = false;
		DesignRoot->LoadInit(SkylineUI, nullptr);
	}
	else {
		SkylineUI->Turtioul = true;
		DesignRoot = UFSDesignMenu::CreateDesignMenu(this, false);
		DesignRoot->SkylineUI = SkylineUI;
	}

	UE_LOG(LogSkyline, Verbose, TEXT("Initializing global Keybinds"));
	FSInput->BindDelegate("MenuKey", FSInputEvent::FS_Pressed, this, &AFSController::onCallMenu);
	FSInput->BindDelegate("EscapeKey", FSInputEvent::FS_Pressed, this, &AFSController::onEscPressed);
	FSInput->BindDelegate("LeftControlKey", FSInputEvent::FS_Pressed, this, &AFSController::onLeftCtrlPressed);
	FSInput->BindDelegate("LeftControlKey", FSInputEvent::FS_Repeat, this, &AFSController::onLeftCtrlPressedRepeat);
	FSInput->BindDelegate("LeftControlKey", FSInputEvent::FS_Released, this, &AFSController::onLeftCtrlReleased);
	FSInput->BindDelegate("FlyingKey", FSInputEvent::FS_Pressed, this, &AFSController::onDoubleSpacePressed);
	FSInput->BindDelegate("TurboSprintKey", FSInputEvent::FS_Pressed, this, &AFSController::onDoubleShiftPressed);
	FSInput->BindDelegate("ScrollUpKey", FSInputEvent::FS_Pressed, this, &AFSController::onMouseScrollUp);
	FSInput->BindDelegate("ScrollDownKey", FSInputEvent::FS_Pressed, this, &AFSController::onMouseScrollDown);
	FSInput->BindDelegate("LeftMouseKey", FSInputEvent::FS_Pressed, this, &AFSController::onLeftMouseDown);
	FSInput->BindDelegate("LeftMouseKey", FSInputEvent::FS_Released, this, &AFSController::onLeftMouseUp);
	FSInput->BindDelegate("RightMouseKey", FSInputEvent::FS_Pressed, this, &AFSController::onRightMouseDown);
	FSInput->BindDelegate("RightMouseKey", FSInputEvent::FS_Repeat, this, &AFSController::onRightMouseDownRepeat);
	FSInput->BindDelegate("RightMouseKey", FSInputEvent::FS_Released, this, &AFSController::onRightMouseUp);
	FSInput->BindDelegate("ActionShortcut1Key", FSInputEvent::FS_Pressed, this, &AFSController::onActionShortcut1);
	FSInput->BindDelegate("DeleteKey", FSInputEvent::FS_Pressed, this, &AFSController::onDesignDelete);
	
	LastLeftCtrlPressed = 0.0f;
	LastRightMouseDown = 0.0f;

	RectangleSelectMode = false;
	LastShowMouseCursor = false;

	LeftCtrlPressed = false;
	LeftMousePressed = false;
	RightMousePressed = false;
	CallMenuPressed = false;

	this->Design = nullptr;

	AFGCharacterPlayer::OnEquipmentEquipped.AddUObject<AFSController>(this, &AFSController::onPlayerEquipmentEquipped);
	AFGCharacterPlayer::OnEquipmentUnequipped.AddUObject<AFSController>(this, &AFSController::onPlayerEquipmentUnequipped);

	// TODO Most recent patch for u6 broke this, find alternative?
	/*
	FScriptDelegate Func1;
	Func1.BindUFunction(this, FName("onBuildGunRecipeChanged"));
	FGBuildGun->mOnRecipeChanged.Add(Func1);

	FScriptDelegate Func2;
	Func2.BindUFunction(this, FName("onBuildGunStateChanged"));
	FGBuildGun->mOnStateChanged.Add(Func2);
	*/

	// TODO This attempt at fix didn't work either
	/*
	if (FGBuildGun != nullptr) {
		FGBuildGun->mOnRecipeChanged.AddDynamic(this, &AFSController::onBuildGunRecipeChanged);

		FGBuildGun->mOnStateChanged.AddDynamic(this, &AFSController::onBuildGunStateChanged);
	}
	*/

	this->SetActorTickEnabled(true);
	Timer.Start();
}

void AFSController::onPlayerEquipmentEquipped(AFGCharacterPlayer* Player, AFGEquipment* Equipment) {
	if (SkylineUI->IsActive) {
		if (Equipment == this->FGBuildGun) {
			if (SkylineUI->GettingStart->AccquireMenuWidget()->GetParent()) {
				SkylineUI->UnexpandStart(false);
			}
			if (!SkylineUI->Turtioul) SkylineUI->FoldDesign();
		}
	}
}

void AFSController::onPlayerEquipmentUnequipped(AFGCharacterPlayer* Player, AFGEquipment* Equipment) {
	if (SkylineUI->IsActive) {
		if (Equipment == this->FGBuildGun) {
			if (State == FSState::Select || State == FSState::SetAnchor || State == FSState::Copy || State == FSState::SetItem) {
				SetOpenState(false);
			}
		}
	}
}

void AFSController::onBuildGunRecipeChanged(TSubclassOf<class UFGRecipe> newRecipe)
{
}

void AFSController::onBuildGunStateChanged(EBuildGunState newState)
{
	if (SkylineUI->IsActive) {
		if (State == FSState::Select || State == FSState::SetAnchor || State == FSState::Copy || State == FSState::SetItem) {
			if (newState == EBuildGunState::BGS_NONE) return;
			if (newState == EBuildGunState::BGS_MENU) {
				UFGBuildGunState* StateVar = this->FGBuildGun->GetBuildGunStateFor(newState);
				StateVar->EndState();
				for (TObjectIterator<UInputComponent> It; It; ++It) {
					if ((*It)->GetOuter() == this->FGBuildGun) {
						this->FGController->PopInputComponent(*It);
					}
				}
			}
			else {
				this->FGBuildGun->GotoMenuState();
				GetPlayer()->UpdateHUDCrosshair();
			}
		}
	}
}

void AFSController::SetOpenState(bool RestoreEquipment)
{
	if (State == FSState::Select) {
		ExitSelectMode(RestoreEquipment);
	}
	if (State == FSState::SetAnchor) {
		ExitSetAnchorMode(RestoreEquipment);
	}
	if (State == FSState::Copy) {
		ExitCopyMode(RestoreEquipment);
	}
	if (State == FSState::SetItem) {
		ExitSetItemMode(RestoreEquipment);
	}

	LeftCtrlPressed = false;
	SetLeftMousePressed(false);
	SetRightMousePressed(false);
}

void AFSController::StartRectangleSelectMode()
{
	LastShowMouseCursor = IsShowMouseCursor();

	ClearFocusBuilding();
	ShowMouseCursor();

	SkylineUI->SelectRect->Box->SetVisibility(ESlateVisibility::Collapsed);
	SkylineUI->SelectRect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	RectangleSelectMode = true;
	RectangleSelecting = false;
}

void AFSController::StartRectangleSelect(const FVector2D& Start, bool IsPositive)
{
	if (RectangleSelecting) return;
	if (!Select->RectSelectStart(Start, IsPositive)) return;

	RectangleSelecting = true;
	RectanglePositive = IsPositive;
	SelectStart = Start;

	GetPlayer()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Custom);

	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	SkylineUI->SelectRect->PanelSlot->SetPosition(Start/ Scale);
	SkylineUI->SelectRect->PanelSlot->SetSize(FVector2D(0.0f, 0.0f));
	SkylineUI->SelectRect->Box->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void AFSController::EndRectangleSelect(bool Valid)
{
	if (!RectangleSelecting) return;
	RectangleSelecting = false;
	SkylineUI->SelectRect->Box->SetVisibility(ESlateVisibility::Collapsed);

	Select->RectSelectExit(Valid);
}

void AFSController::ExitRectangleSelectMode()
{
	EndRectangleSelect(false);

	if (SkylineUI->SelectRect->GetVisibility() == ESlateVisibility::SelfHitTestInvisible) {
		SkylineUI->SelectRect->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (RectangleSelectMode) {
		if (SkylineUI->IsActive && State == FSState::Select) {
			if (LastShowMouseCursor) {
				ShowMouseCursor();
			}
			else {
				HideMouseCursor();
			}
		}
		RectangleSelectMode = false;
		LastShowMouseCursor = false;
	}

	CheckFlying();
}

void AFSController::StartSelectMode()
{
	if (State != FSState::Open) SetOpenState(true);

	if (GetPlayer()->IsBuildGunEquipped()) GetPlayer()->ToggleBuildGun();

	State = FSState::Select;
	this->SkylineUI->OperatingWidget->OnTab2Click();

	EquipBuilder();
	PopFGUI();

	HideMouseCursor();

	SelectWaitingResult = false;
	ConnectSelectMode = true;
	ChangeConnectSelectMode();

	this->SkylineUI->RestoreHighLightMapping();
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->HoldCtrlMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->SelectScrollMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->SelectMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->SelectAnchorMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->GotoBuildMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	this->Select->LoadSelect(this->Design);
	CurrentFocusBuilding = nullptr;
}


void AFSController::ExitSelectMode(bool RestoreEquipment)
{
	ExitRectangleSelectMode();

	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->HoldCtrlMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SelectScrollMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SelectMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SelectAnchorMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->GotoBuildMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::Collapsed);

	this->ClearFocusBuilding();
	this->Select->Unload();

	SelectOne = false;
	State = FSState::Open;

	if (RestoreEquipment) UnequipBuilder();
}

void AFSController::StartCopyMode()
{
	if (State != FSState::Open) SetOpenState(true);

	if (GetPlayer()->IsBuildGunEquipped()) GetPlayer()->ToggleBuildGun();

	State = FSState::Copy;
	this->SkylineUI->OperatingWidget->OnTab3Click();

	EquipBuilder();
	PopFGUI();

	HideMouseCursor();

	this->SkylineUI->RestoreHighLightMapping();
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->CopyScrollMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->CopyMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	this->Builder->Load(this->Design, this->FullPreview);
}

void AFSController::ExitCopyMode(bool RestoreEquipment)
{
	this->SkylineUI->RepeatWidget->SetVisibility(ESlateVisibility::Hidden);
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->CopyScrollMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->CopyMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SetRepeatMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::Collapsed);

	this->SkylineUI->ItemBox->SetVisibility(ESlateVisibility::Collapsed);

	this->Builder->Unload();

	BuildOne = false;
	CopyPosition = false;
	CopyConsole = false;
	BuildRepeatCallBack = false;
	State = FSState::Open;

	if (RestoreEquipment) UnequipBuilder();
}

void AFSController::StartSetAnchorMode()
{
	if (State != FSState::Open) SetOpenState(true);

	if (GetPlayer()->IsBuildGunEquipped()) GetPlayer()->ToggleBuildGun();

	State = FSState::SetAnchor;
	this->SkylineUI->OperatingWidget->OnTab3Click();

	EquipBuilder();
	PopFGUI();

	HideMouseCursor();

	this->SkylineUI->RestoreHighLightMapping();
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->SetAnchorMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	this->Select->LoadSetAnchor(this->Design);
	CurrentFocusBuilding = nullptr;
}

void AFSController::ExitSetAnchorMode(bool RestoreEquipment)
{
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SetAnchorMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->BackToSelectMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::Collapsed);

	this->Select->Unload();

	State = FSState::Open;

	if (RestoreEquipment) UnequipBuilder();
}

void AFSController::StartSetItemMode()
{
	if (State != FSState::Open) SetOpenState(true);

	if (GetPlayer()->IsBuildGunEquipped()) GetPlayer()->ToggleBuildGun();

	State = FSState::SetItem;

	EquipBuilder();
	PopFGUI();

	HideMouseCursor();

	this->SkylineUI->RestoreHighLightMapping();
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->SetCircleCenterMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	this->Select->Load(this->Design);
	CurrentFocusBuilding = nullptr;
}

void AFSController::ExitSetItemMode(bool RestoreEquipment)
{
	this->SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
	this->SkylineUI->PressCtrlMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->SetCircleCenterMapping->SetVisibility(ESlateVisibility::Collapsed);
	this->SkylineUI->ExitMapping->SetVisibility(ESlateVisibility::Collapsed);

	this->Select->Unload();

	State = FSState::Open;

	if (RestoreEquipment) UnequipBuilder();
	CurrentFocusBuilding = nullptr;
}

void AFSController::SetLeftMousePressed(bool Pressed)
{
	LeftMousePressed = Pressed;

	if (State == FSState::Select)
		if (SkylineUI->SelectMapping->IsHighLight != Pressed) {
			if (Pressed) SkylineUI->SetHighLightMapping(SkylineUI->SelectMapping);
			else SkylineUI->RestoreHighLightMapping();
		}

	if (State == FSState::Copy)
		if (SkylineUI->CopyMapping->IsHighLight != Pressed) {
			if (Pressed) SkylineUI->SetHighLightMapping(SkylineUI->CopyMapping);
			else SkylineUI->RestoreHighLightMapping();
		}

	if (State == FSState::SetAnchor)
		if (SkylineUI->SetAnchorMapping->IsHighLight != Pressed) {
			if (Pressed) SkylineUI->SetHighLightMapping(SkylineUI->SetAnchorMapping);
			else SkylineUI->RestoreHighLightMapping();
		}

	if (State == FSState::SetItem)
		if (SkylineUI->SetCircleCenterMapping->IsHighLight != Pressed) {
			if (Pressed) SkylineUI->SetHighLightMapping(SkylineUI->SetCircleCenterMapping);
			else SkylineUI->RestoreHighLightMapping();
		}
}

void AFSController::SetRightMousePressed(bool Pressed)
{
	RightMousePressed = Pressed;
	if (SkylineUI->ExitMapping->IsHighLight != Pressed) {
		if (Pressed) SkylineUI->SetHighLightMapping(SkylineUI->ExitMapping);
		else SkylineUI->RestoreHighLightMapping();
	}
}

void AFSController::Tick(float dt)
{
	if (!this->World) return;

	FSInput->Tick();

	if (SkylineUI->IsActive) {
		if (State == FSState::Select) {
			TickSelect(dt);
		}
		if (State == FSState::Copy && !CopyWaitingResult) {
			TickCopy(dt);
		}
		if (State == FSState::SetItem) {
			TickSetItem(dt);
		}
		if (SkylineUI->Logo->IsShow && !SkylineUI->GettingStart->AccquireMenuWidget()->GetParent() && SkylineUI->Logo->Time.GetTime() > 15.0) {
			SkylineUI->Logo->StartFadeAnimation();
		}
	}

	if (CopyWaitingResult) {
		if (this->Builder->CheckReady(0.003f)) {
			CopyWaitingResult = false;
			SkylineUI->MessageBox->SetVisibility(ESlateVisibility::Hidden);
			this->onBuildFinish();
		}
		else {
			SkylineUI->Message->SetText(FText::FromString(FString::Printf(TEXT("Waiting for building %d / %d"), Builder->SyncBuild->GetCurrent(), Builder->SyncBuild->GetTotal())));
		}
	}

	if (GetPlayer()) {
		if (Etc->GetBool("IsFlying")) {
			EMovementMode MovementMode = GetPlayer()->GetCharacterMovement()->MovementMode;
			if (MovementMode != EMovementMode::MOVE_Custom) {
				if (MovementMode != EMovementMode::MOVE_Flying) CheckFlying();
				else TickFly(dt);
			}
		}

		if (Etc->GetBool("InfiniteAmmo")) {
			AFGWeapon* Weapon = Cast<AFGWeapon>(GetPlayer()->GetEquipmentInSlot(EEquipmentSlot::ES_ARMS));
			if (Weapon) {
				Weapon->mCurrentAmmoCount = Weapon->GetMagSize();
			}
		}
	}

	if (Etc->GetBool("DisableHighSpaceFog")) {
		UExponentialHeightFogComponent* Fog = this->WorldSettings->GetExponentialHeightFog()->GetComponent();
		if (Fog->FogDensity > 0.02) Fog->FogDensity = 0.02;
		if (Fog->GetComponentLocation().Z > -1500.0 + 1.0) {
			if (Fog->FogHeightFalloff < 0.18) Fog->FogHeightFalloff = 0.18;
			Fog->SetWorldLocation(FVector(0.0, 0.0, -1500.0));
		}
	}

	if (FGController->IsMoveInputIgnored() && !this->FSInput->IsKeyDown("LeftControlKey"))
		ExitRectangleSelectMode();

	if (TurboSprint && !this->FSInput->IsKeyDown("TurboSprintKey")) TurboSprint = false;
}

void AFSController::TickSelect(float dt)
{
	if (SelectWaitingResult) {
		if (this->Select->ConnectSelectCheckReady()) {
			SelectWaitingResult = false;
		}
	}
	if (!SelectWaitingResult && !RectangleSelectMode) {
		FHitResult Hit = this->GetSelectHitResult();
		AFGBuildable* Building = this->AcquireBuildable(Hit);

		if (Building != this->CurrentFocusBuilding.Get()) {
			if (this->CurrentFocusBuilding.Get()) this->ClearFocusBuilding();

			if (Building) {

				std::string  str = TCHAR_TO_UTF8(*Building->GetName());
				str.append("\n");
				str.append("FOUND HIT\n");
				std::wstring temp = std::wstring(str.begin(), str.end());
				LPCWSTR wideString = temp.c_str();
				OutputDebugStringW(wideString);

				if (this->FSInput->IsKeyDown("LeftMouseKey")) {
					if (!this->FSInput->IsKeyDown("RightMouseKey")) {
						if (!ConnectSelectMode) {
							if (!this->Design->IsElementSelected(Building)) {

								this->Select->Select(Building);
								LeftMousePressed = false;
							}
						}
					}
				}
				else {
					this->SetFocusBuilding(Building);
				}
			}
		}
	}
}

void AFSController::TickSetItem(float dt)
{
	FHitResult Hit = this->GetSelectHitResult();
	AFGBuildable* Building = this->AcquireBuildable(Hit);
	if (Building != this->CurrentFocusBuilding.Get()) {
		if (this->CurrentFocusBuilding.Get()) this->ClearFocusBuilding();
		if (Building) this->SetFocusBuilding(Building);
	}
}

void AFSController::TickCopy(float dt)
{
	if (!CopyConsole) {
		FHitResult Hit = this->GetCopyHitResult();
		this->Builder->Update(Hit);
	}
	else {
		this->Builder->Update(&this->Repeat);
	}
}

void AFSController::TickFly(float dt)
{
	if (!GetPlayer()) return;

	FVector CamLoc;
	FRotator CamRot;
	this->FGController->GetPlayerViewPoint(CamLoc, CamRot);
	FQuat CamQuat = CamRot.Quaternion();

	float Speed = 2000.0f;
	if (this->FSInput->IsKeyDown("SprintKey")) Speed = 4000.0f;
	if (TurboSprint) Speed = 20000.0f;
	FVector Velocity = FVector::ZeroVector;

	if (!FGController->IsMoveInputIgnored()) {
		if (this->FSInput->IsKeyDown("ForwardKey")) Velocity += CamQuat.GetAxisX();
		if (this->FSInput->IsKeyDown("BackwardKey")) Velocity -= CamQuat.GetAxisX();
		if (this->FSInput->IsKeyDown("LeftKey")) Velocity -= CamQuat.GetAxisY();
		if (this->FSInput->IsKeyDown("RightKey")) Velocity += CamQuat.GetAxisY();
		if (this->FSInput->IsKeyDown("GoUpKey")) Velocity += FRotator::ZeroRotator.Quaternion().GetAxisZ();
		if (this->FSInput->IsKeyDown("GoDownKey")) Velocity -= FRotator::ZeroRotator.Quaternion().GetAxisZ();
	}

	Velocity.Normalize(1.0f);
	GetPlayer()->GetCharacterMovement()->Velocity = Velocity * Speed;
}

void AFSController::LoadDesign(UFSDesign* DesignParam)
{
	if (this->Design && this->Design != DesignParam) UnloadDesign(false);
	this->Design = DesignParam;
	Repeat.Ready = false;
}

void AFSController::UnloadDesign(bool ShowMouse)
{
	this->Design = nullptr;
	SetOpenState(true);
	if (ShowMouse) this->ShowMouseCursor();
}

void AFSController::onCallMenu() {
	ExecCallMenu();
}

void AFSController::onEscPressed()
{
	if (SkylineUI->IsActive) {
		SetOpenState(true);

		State = FSState::Close;
		SkylineUI->HideMenu();
	}
}

void AFSController::onLeftCtrlPressed()
{
	LastLeftCtrlPressed = World->GetRealTimeSeconds();
	LeftCtrlPressed = true;
	if (SkylineUI->IsActive) {
		if (State == FSState::Select && !SelectWaitingResult) {
			StartRectangleSelectMode();
			SkylineUI->SetHighLightMapping(SkylineUI->PressCtrlMapping);
		}
		else if (State == FSState::SetItem || State == FSState::SetAnchor || State == FSState::Copy) {
			SkylineUI->SetHighLightMapping(SkylineUI->PressCtrlMapping);
		}
	}
}

void AFSController::onLeftCtrlPressedRepeat()
{
	if (RectangleSelectMode) {
		if (World->GetRealTimeSeconds() - LastLeftCtrlPressed >= ShortPressLeftCtrlTimeLimit)
			if (!SkylineUI->HoldCtrlMapping->IsHighLight)
				SkylineUI->SetHighLightMapping(SkylineUI->HoldCtrlMapping);
	}
	
	if (State == FSState::SetItem || State == FSState::SetAnchor || State == FSState::Copy) {
		if (World->GetRealTimeSeconds() - LastLeftCtrlPressed >= ShortPressLeftCtrlTimeLimit)
			SkylineUI->RestoreHighLightMapping();
	}
}

void AFSController::onLeftCtrlReleased()
{
	ExitRectangleSelectMode();
	if (!LeftCtrlPressed) return;
	LeftCtrlPressed = false;

	if (World->GetRealTimeSeconds() - LastLeftCtrlPressed < ShortPressLeftCtrlTimeLimit) {
		if (IsShowMouseCursor()) {
			HideMouseCursor();
		}
		else {
			ShowMouseCursor();
		}
	}
	SkylineUI->RestoreHighLightMapping();
}

void AFSController::onDoubleSpacePressed()
{
	if (Etc->GetBool("DoubleSpaceFlying")) {
		Etc->SetBool("IsFlying", !Etc->GetBool("IsFlying"));
		CheckFlying();
	}
}

void AFSController::onDoubleShiftPressed()
{
	if (Etc->GetBool("DoubleShiftSprint")) {
		TurboSprint = true;
	}
}

void AFSController::onMouseScrollUp()
{
	if (LeftCtrlPressed) {

	}
	else {
		if (SkylineUI->IsActive) {
			if (State == FSState::Select) {
				ChangeConnectSelectMode();
			}
		}
	}

	if (SkylineUI->IsActive) {
		if (State == FSState::Copy && !CopyWaitingResult && !CopyConsole) {
			this->Builder->ScrollUp();
		}
	}

	SetLeftMousePressed(false);
	SetRightMousePressed(false);
}

void AFSController::onMouseScrollDown()
{
	if (LeftCtrlPressed) {

	}
	else {
		if (SkylineUI->IsActive) {
			if (State == FSState::Select) {
				ChangeConnectSelectMode();
			}
		}
	}

	if (SkylineUI->IsActive) {
		if (State == FSState::Copy && !CopyWaitingResult && !CopyConsole) {
			this->Builder->ScrollDown();
		}
	}

	SetLeftMousePressed(false);
	SetRightMousePressed(false);
}

void AFSController::onLeftMouseDown()
{
	if (!SkylineUI->IsActive) return;

	SetLeftMousePressed(false);

	if (LeftCtrlPressed) {
	}
	else {
		if (!SelectWaitingResult) {
			if (State == FSState::Select) {
				if (this->FSInput->IsKeyDown("RightMouseKey")) {
					onSelectModeLeftRightMouseClickWithoutLeftCtrl();
					SkylineUI->SetHighLightMapping(SkylineUI->SelectAnchorMapping);
				}
				else {
					if (!ConnectSelectMode) {
						AFGBuildable* Building = CurrentFocusBuilding.Get();
						if (Building && !this->Design->IsElementSelected(Building)) {
							this->Select->Select(Building);
							SkylineUI->SetHighLightMapping(SkylineUI->SelectMapping);
						}
						else SetLeftMousePressed(true);
					}
					else SetLeftMousePressed(true);
				}
			}
			else SetLeftMousePressed(true);
		}
	}
	SetRightMousePressed(false);
}

void AFSController::onLeftMouseUp()
{
	if (!SkylineUI->IsActive) return;

	if (LeftCtrlPressed) {
	}
	else {
		if (LeftMousePressed) {
			if (State == FSState::Select && !SelectWaitingResult) {
				if (CurrentFocusBuilding.Get()) {
					if (ConnectSelectMode) {
						if (this->Select->ConnectSelect(CurrentFocusBuilding.Get())) {
							SelectWaitingResult = true;
						}
					}
					else this->Select->Select(CurrentFocusBuilding.Get());

					if (Design->Anchor.Get() && !Design->IsElementSelected(Design->Anchor.Get())) {
						Design->Anchor = nullptr;
						CheckAnchor(false);
					}
				}
			}
			else if (State == FSState::SetAnchor) {
				FHitResult Hit = this->GetCopyHitResult();
				AFGBuildable* Building = this->AcquireBuildable(Hit);

				if (this->Design->IsElementSelected(Building)) {
					this->Design->Anchor = Building;
					if (CheckAnchor(true)) {
						SetOpenState(true);
						StartCopyMode();
					}
				}
			}
			else if (State == FSState::SetItem) {
				if (CurrentFocusBuilding.Get()) {
					SkylineUI->OperatingWidget->AdvancedCopyPanel->SetCircleFeedback();
				}
			}
			else if (State == FSState::Copy && !CopyWaitingResult && !CopyConsole) {
				if (CopyPosition) {
					this->onSetRepeat();
				}
				else if (this->Builder->Build(nullptr)) {
					CopyWaitingResult = true;
					SkylineUI->MessageBox->SetVisibility(ESlateVisibility::Visible);
					//this->onBuildFinish();
				}
			}
		}
		SkylineUI->RestoreHighLightMapping();
	}
	SetLeftMousePressed(false);
	SetRightMousePressed(false);
}

void AFSController::onRightMouseDown()
{
	LastRightMouseDown = World->GetRealTimeSeconds();
	if (!SkylineUI->IsActive) return;

	if (LeftCtrlPressed) {
	}
	else {
		if (State == FSState::Select && !SelectWaitingResult && this->FSInput->IsKeyDown("LeftMouseKey")) {
			onSelectModeLeftRightMouseClickWithoutLeftCtrl();
			SkylineUI->SetHighLightMapping(SkylineUI->SelectAnchorMapping);
		}
		else SetRightMousePressed(true);
	}
	SetLeftMousePressed(false);
}

void AFSController::onRightMouseDownRepeat()
{
	if (!SkylineUI->IsActive) return;
	if (LeftCtrlPressed) {

	}
	else {
		if (RightMousePressed) {
			if (World->GetRealTimeSeconds() - LastRightMouseDown >= ShortPressMouseRightTimeLimit) {
				SetRightMousePressed(false);
			}
		}
	}
}

void AFSController::onRightMouseUp()
{
	if (!SkylineUI->IsActive) return;

	if (LeftCtrlPressed) {
	}
	else {
		if (RightMousePressed && (State == FSState::Select || State == FSState::SetAnchor || State == FSState::SetItem || State == FSState::Copy && !CopyConsole)) {
			
			if (World->GetRealTimeSeconds() - LastRightMouseDown < ShortPressMouseRightTimeLimit) {
				SetOpenState(true);
				ShowMouseCursor();
			}
		}
		SkylineUI->RestoreHighLightMapping();
	}
	SetLeftMousePressed(false);
	SetRightMousePressed(false);
}

void AFSController::onActionShortcut1()
{
	if (!SkylineUI->IsActive) return;

	if (State == FSState::Select) {
		SetOpenState(true);
		if (CheckAnchor(true)) StartCopyMode();
		else StartSetAnchorMode();
	}
	else if (State == FSState::SetAnchor) {
		SetOpenState(true);
		StartSelectMode();
	}
	else if (State == FSState::Copy && !BuildOne && !CopyPosition && !CopyConsole) {
		SetOpenState(true);
		StartSelectMode();
	}
}

void AFSController::onSelectModeLeftRightMouseClickWithoutLeftCtrl()
{
	if (CurrentFocusBuilding.Get()) {
		if (Design->Anchor == CurrentFocusBuilding) {
			Design->Anchor = nullptr;
			this->Select->DisableHightLightFocus();
		}
		else {
			if (!Design->IsElementSelected(CurrentFocusBuilding.Get())) {
				this->Select->Select(CurrentFocusBuilding.Get());
			}
			if (Design->Anchor.Get()) this->Select->EnableHightLight(Design->Anchor.Get(), Select->SelectMaterial);
			Design->Anchor = CurrentFocusBuilding;
			this->Select->EnableHightLightFocus(CurrentFocusBuilding.Get());
		}
		CheckAnchor(false);
	}
}

void AFSController::onMouseMove(const FVector2D& MousePosition)
{
	if (RectangleSelecting) {
		FVector2D Start = SelectStart;
		FVector2D End = MousePosition;
		if (End.X < Start.X) Swap(End.X, Start.X);
		if (End.Y < Start.Y) Swap(End.Y, Start.Y);

		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		this->SkylineUI->SelectRect->PanelSlot->SetPosition(Start / Scale);
		this->SkylineUI->SelectRect->PanelSlot->SetSize((End - Start) / Scale);

		if (Select->RectSelectCheckReady()) {
			Select->RectSelectUpdate(MousePosition);
		}
	}
}

void AFSController::onDesignDelete()
{
	if (!SkylineUI->IsActive) return;
	UFSDesignMenu* DesignMenu = SkylineUI->FindLastDesignItem();

	if (DesignMenu != SkylineUI->DesignRoot) {
		UFSDesignMenu* Parent = Cast<UFSDesignMenu>(DesignMenu->ParentItem);
		if (Parent) {
			if (DesignMenu->AccquireEntry() == SkylineUI->CurrentHighLightButton)
				SkylineUI->CurrentHighLightButton = nullptr;

			if (Parent->Folded) DesignMenu->Unfold();

			DesignMenu->Unexpand();

			Parent->RemoveChild(DesignMenu);
		}
	}
}

void AFSController::onPreResourceSinkTick(AFGBuildableResourceSink* Sink)
{
	if (!RecycleMaterials || !Sink) return;
	UFGFactoryConnectionComponent* Input = Cast<UFGFactoryConnectionComponent>(Sink->GetComponentByClass(UFGFactoryConnectionComponent::StaticClass()));

	FScopeLock ScopeLock(&Mutex);

	FInventoryItem out_item;
	float out_OffsetBeyond;
	if (Input && Input->Factory_GrabOutput(out_item, out_OffsetBeyond)) {
		UE_LOG(LogSkyline, Verbose, TEXT("%s"), *Sink->GetFullName());
		UE_LOG(LogSkyline, Verbose, TEXT("%s"), *out_item.ItemClass->GetFullName());
		//ResourceSinkSubsystem->AddPoints_ThreadSafe(out_item.ItemClass);
		Inventory.AddResource(out_item.ItemClass, 1);
	}
	
	/*
	TArray< FInventoryItem > out_items;
	if (Input->Factory_PeekOutput(out_items)) {
		if (out_items.Num() > 0) {
			SML::Logging::info(*Sink->GetFullName());
			SML::Logging::info(*out_items[0].ItemClass->GetFullName());
			
			if (ResourceSinkSubsystem->GetResourceSinkPointsForItem(out_items[0].ItemClass) == 0) {
				FInventoryItem out_item;
				float out_OffsetBeyond;
				Input->Factory_GrabOutput(out_item, out_OffsetBeyond);
			}
			Inventory.AddResource(out_items[0].ItemClass, 1);
		}
	}*/
}

bool AFSController::IsShowMouseCursor()
{
	return FGController->bShowMouseCursor;
}

void AFSController::ShowMouseCursor()
{
	UFGVirtualCursorFunctionLibrary::EnableVirtualCursor(FGController);
	FGController->bShowMouseCursor = true;
}

void AFSController::HideMouseCursor()
{
	UFGVirtualCursorFunctionLibrary::DisableVirtualCursor(FGController);
	FGController->bShowMouseCursor = false;
}

void AFSController::onBuildFinish()
{
	if (State == FSState::Copy) {
		if (BuildRepeatCallBack) {
			SkylineUI->RepeatWidget->CallBack();
		}
		if (BuildOne) {
			ExitCopyMode(true);
			ShowMouseCursor();
			Repeat.Source = Builder->Source;
			Repeat.Target = Builder->Target;
			Repeat.Ready = true;
		}
	}
}

void AFSController::onSetRepeat()
{
	if (State == FSState::Copy && CopyPosition && Builder->LastShow && (!Repeat.Ready || Repeat.Source.Equals(Builder->Source))) {
		if (Repeat.Ready) {
			Repeat.NextRelative.SetLocation(Repeat.Target.InverseTransformPositionNoScale(Builder->Target.GetLocation()));
			Repeat.NextRelative.SetRotation(Repeat.Target.InverseTransformRotation(Builder->Target.GetRotation()));
			SkylineUI->OperatingWidget->AdvancedCopyPanel->SetTransform(Repeat.NextRelative);
			SkylineUI->OperatingHelper->SetVisibility(ESlateVisibility::Hidden);
			SkylineUI->RepeatWidget->Load();
			SkylineUI->RepeatWidget->StartEnterAnimation();
			CopyPosition = false;
			CopyConsole = true;
		}
		else {
			Repeat.Source = Builder->Source;
			Repeat.Target = Builder->Target;
			Repeat.Ready = true;
			ExitCopyMode(true);
		}
		ShowMouseCursor();
	}
}

void AFSController::ExecCallMenu()
{
	//UExponentialHeightFogComponent* Fog = this->WorldSettings->GetExponentialHeightFog()->GetComponent();
	//SML::Logging::info(Fog->FogDensity, TEXT(" "), Fog->FogHeightFalloff, TEXT(" "), *Fog->GetComponentLocation().ToString());

	if (SkylineUI->IsActive) {
		if (State == FSState::Select || State == FSState::SetAnchor || State == FSState::SetItem || State == FSState::Copy) {
			SetOpenState(true);
			ShowMouseCursor();
		}
		else {
			if (SkylineUI->GettingStart->AccquireMenuWidget()->GetParent()) {
				SkylineUI->UnexpandStart(true);
			}
			else {
				SkylineUI->ExpandStart();
				ShowMouseCursor();
			}
		}
	}
	else {
		SkylineUI->OpenMenu();
		State = FSState::Open;
		ShowMouseCursor();
	}

}

void AFSController::EquipBuilder() {

	GetPlayer()->SetOverrideEquipment(this->FGBuildGun);
}

void AFSController::UnequipBuilder() {
	AFGEquipment* Equipment = GetPlayer()->GetEquipmentInSlot(EEquipmentSlot::ES_ARMS);
	if (Equipment == this->FGBuildGun) {
		GetPlayer()->ClearOverrideEquipment(this->FGBuildGun);
	}

}

void AFSController::PopFGUI() {
	//this->FGUI->PopAllWidgets();
}

void AFSController::CheckFlying()
{
	//SML::Logging::info(*GetPlayer()->GetFullName());
	if (!GetPlayer()) return;
	UCharacterMovementComponent* MovementComponent = GetPlayer()->GetCharacterMovement();
	if (!MovementComponent) return;
	if (Etc->GetBool("IsFlying")) {
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		MovementComponent->MaxFlySpeed = 20000.0f;
		MovementComponent->MaxAcceleration = 0.0f;
	}
	else {
		MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		MovementComponent->MaxAcceleration = 10000.0f;
	}
}

bool AFSController::CheckAnchor(bool Warn)
{
	if (this->Design->Anchor.Get()) {
		if (this->Builder->CheckAnchor(this->Design)) {
			this->Design->SetItemFeedback->SetText(FText::FromString(this->Design->Anchor.Get()->GetName()));
			this->Design->SetItemFeedback->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
			this->Design->SetItemFeedback->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			return true;
		}
		else {
			this->Design->SetItemFeedback->SetText(FText::FromString(TEXT("Anchor Not Valid")));
			this->Design->SetItemFeedback->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
			this->Design->SetItemFeedback->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			return false;
		}
	}
	if (Warn) {
		this->Design->SetItemFeedback->SetText(FText::FromString(TEXT("Anchor Not Found")));
		this->Design->SetItemFeedback->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)));
		this->Design->SetItemFeedback->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		this->Design->SetItemFeedback->SetVisibility(ESlateVisibility::Collapsed);
	}
	return false;
}

AFGBuildable* AFSController::AcquireBuildable(const FHitResult& Hit)
{
	if (Hit.GetActor()) {
		AFGBuildable* Buildable = Cast<AFGBuildable>(Hit.GetActor());
		if (Buildable && !Buildable->IsPlayingBuildEffect()) return Buildable;
		return nullptr;
	}
	return nullptr;
}

FHitResult AFSController::GetSelectHitResult()
{
	if (IsShowMouseCursor()) return GetMouseCursorHitResult(true);

	FHitResult Hit;
	
	Hit = this->FGBuildGun->GetHitResult();
	if (AcquireBuildable(Hit)) return Hit;	
	

	FVector CamLoc;
	FRotator CamRot;
	this->FGController->GetPlayerViewPoint(CamLoc, CamRot);

	float max = 1000.0f;
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * DistanceMax);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, GetPlayer());
	TraceParams.AddIgnoredActor(this->FGBuildGun);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	if (this->World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams)) {
		if (AcquireBuildable(Hit)) {
			return Hit;
		}
	}

	return FHitResult();
}

FHitResult AFSController::GetCopyHitResult()
{
	if (IsShowMouseCursor()) return GetMouseCursorHitResult(false);

	FHitResult Hit;

	FVector CamLoc;
	FRotator CamRot;
	this->FGController->GetPlayerViewPoint(CamLoc, CamRot);

	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * DistanceMax);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, GetPlayer());
	TraceParams.AddIgnoredActor(this->FGBuildGun);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	if (this->World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams)) {
		return Hit;
	}

	return FHitResult();
}

FHitResult AFSController::GetMouseCursorHitResult(bool RequireBuildable)
{
	if (this->SkylineUI->IsMouseCursorOnUI) return FHitResult();

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, GetPlayer());
	TraceParams.AddIgnoredActor(this->FGBuildGun);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	float x, y;
	if (this->FGController->GetMousePosition(x, y)) {
		FVector2D MousePosition(x, y);

		if (HUD->GetHitBoxAtCoordinates(MousePosition, true)) return FHitResult();

		FVector WorldOrigin;
		FVector WorldDirection;
		if (UGameplayStatics::DeprojectScreenToWorld(this->FGController, MousePosition, WorldOrigin, WorldDirection))
		{
			FHitResult Hit;
			if (this->World->LineTraceSingleByChannel(Hit, WorldOrigin, WorldOrigin + WorldDirection * DistanceMax, ECC_Visibility, TraceParams)) {
				if (!RequireBuildable || AcquireBuildable(Hit)) {
					return Hit;
				}
			}
		}
	}

	return FHitResult();
}

void AFSController::SetFocusBuilding(AFGBuildable* Buildable)
{
	//this->GetPlayer()->GetOutline()->ShowOutline(Buildable, EOutlineColor::OC_HOLOGRAM);
	//this->GetPlayer()->GetOutline()->ShowDismantlePendingMaterial(Buildable);
	CurrentFocusBuilding = Buildable;
	this->Select->EnableHightLightFocus(CurrentFocusBuilding.Get());
}

void AFSController::ClearFocusBuilding()
{
	if (CurrentFocusBuilding.Get()) {
		this->Select->DisableHightLightFocus();
	}
	CurrentFocusBuilding = nullptr;
}

void AFSController::ChangeConnectSelectMode()
{
	if (ConnectSelectMode) {
		ConnectSelectMode = false;
		this->SkylineUI->SelectScrollMapping->SetTitle(FText::FromString(TEXT("Select One")));
	}
	else {
		ConnectSelectMode = true;
		this->SkylineUI->SelectScrollMapping->SetTitle(FText::FromString(TEXT("Select Connected")));
	}
}

void AFSController::onSwitchFlying()
{
	if (!Etc->GetBool("DoubleSpaceFlying")) {
		Etc->SetBool("IsFlying", false);
		CheckFlying();
	}
}

void AFSController::onSwitchFog()
{
	this->WorldSettings->GetExponentialHeightFog()->SetActorHiddenInGame(Etc->GetBool("DisableFog"));
}

void AFSController::onSwitchGodlike()
{
	UFGHealthComponent* HealthComponent = GetPlayer()->GetHealthComponent();
	HealthComponent->Heal(HealthComponent->GetMaxHealth() - HealthComponent->GetCurrentHealth());
	GetPlayer()->bCanBeDamaged = !Etc->GetBool("Godlike");
}

void AFSController::onSwitchThirdPersonView()
{
	if (Etc->GetBool("ThirdPersonView"))
		GetPlayer()->SetThirdPersonMode();
	else
		GetPlayer()->SetFirstPersonMode();
}

void AFSController::onSwitchRecycleMaterials()
{
	this->RecycleMaterials = Etc->GetBool("RecycleMaterials");
}

AFGCharacterPlayer* AFSController::GetPlayer()
{
	if (CurrentPlayer.Get()) return CurrentPlayer.Get();

	for (TActorIterator<AFGCharacterPlayer>It(World); It; ++It) {
		if (It->IsLocallyControlled()) {
			CurrentPlayer = *It;
			this->FGBuildGun = GetPlayer()->GetBuildGun();
			//onSwitchThirdPersonView();
			return *It;
		}
	}
	//SML::Logging::info(TEXT("AFSController::GetPlayer"));
	return nullptr;
}