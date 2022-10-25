// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Core/Public/HAL/ThreadingBase.h"
#include "UI/FSkylineUI.h"
#include "FSSelection.h"
#include "FSBuilder.h"
#include "FSDesign.h"
#include "FSInput.h"
#include "FSInventory.h"
#include "FSBuildableService.h"
#include "FGPlayerController.h"
#include "FGSaveInterface.h"
#include "FGWorldSettings.h"
#include "FGResourceSinkSubsystem.h"
#include "Equipment/FGEquipment.h"
#include "Buildables/FGBuildableResourceSink.h"
#include "UI/FGGameUI.h"
#include "FSController.generated.h"


enum FSState
{
	Close,
	Open,
	Select,
	SetAnchor,
	SetItem,
	Copy
};

UCLASS()
class FACTORYSKYLINE_API UFSEtc : public UObject, public IFGSaveInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame)
	TMap<FName, FString> Attribute;

	virtual bool ShouldSave_Implementation() const override { return !GetBool("DontSaveAnything"); }

	FString Get(const FName& Key) const
	{
		const FString* Result = Attribute.Find(Key);
		if (!Result) return TEXT("");
		return *Result;
	}
	bool GetBool(const FName& Key) const
	{
		const FString* Result = Attribute.Find(Key);
		if (!Result) return false;
		return Result->Equals("True", ESearchCase::IgnoreCase) ? true : false;
	}
	void Set(const FName& Key, const FString& Value)
	{
		Attribute.Add(Key, Value);
	}
	void SetBool(const FName& Key, bool Value)
	{
		Set(Key, Value ? TEXT("True") : TEXT("False"));
	}
	bool HasKey(const FName& Key) const
	{
		return Attribute.Find(Key) != nullptr;
	}
	void Init()
	{
		if (!HasKey("IsFlying")) SetBool("IsFlying", false);
		if (!HasKey("DoubleSpaceFlying")) SetBool("DoubleSpaceFlying", false);
		if (!HasKey("DoubleShiftSprint")) SetBool("DoubleShiftSprint", false);
		if (!HasKey("DisableFog")) SetBool("DisableFog", false);
		if (!HasKey("DisableHighSpaceFog")) SetBool("DisableHighSpaceFog", false);
		if (!HasKey("Godlike")) SetBool("Godlike", false);
		if (!HasKey("ThirdPersonView")) SetBool("ThirdPersonView", false);
		if (!HasKey("InfiniteAmmo")) SetBool("InfiniteAmmo", false);
		if (!HasKey("DontSaveAnything")) SetBool("DontSaveAnything", false);
		if (!HasKey("RecycleMaterials")) SetBool("RecycleMaterials", false);
	}
};

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API AFSController : public AFGEquipment
{
	GENERATED_BODY()
public:

	virtual bool ShouldSave_Implementation() const override { return Etc ? !Etc->GetBool("DontSaveAnything") : false; }

	void Init();
	void LoadDesign(UFSDesign* DesignParam);
	void UnloadDesign(bool ShowMouse);
	
	void onCallMenu();
	void onEscPressed();
	void onLeftCtrlPressed();
	void onLeftCtrlPressedRepeat();
	void onLeftCtrlReleased(); 
	void onDoubleSpacePressed();
	void onDoubleShiftPressed();
	void onMouseScrollUp();
	void onMouseScrollDown();
	void onLeftMouseDown();
	void onLeftMouseUp();
	void onRightMouseDown();
	void onRightMouseDownRepeat();
	void onRightMouseUp();
	void onActionShortcut1();
	void onSelectModeLeftRightMouseClickWithoutLeftCtrl();
	void onMouseMove(const FVector2D& MousePosition);
	void onDesignDelete();

	void onPreResourceSinkTick(AFGBuildableResourceSink* Sink);
	
	bool IsShowMouseCursor();
	void ShowMouseCursor();
	void HideMouseCursor();

	void onPlayerEquipmentEquipped(AFGCharacterPlayer* Player, AFGEquipment* Equipment);
	void onPlayerEquipmentUnequipped(AFGCharacterPlayer* Player, AFGEquipment* Equipment);
	
	UFUNCTION()
	void onBuildGunRecipeChanged(TSubclassOf< class UFGRecipe > newRecipe);

	UFUNCTION()
	void onBuildGunStateChanged(EBuildGunState newState);

	void SetOpenState(bool RestoreEquipment);

	void StartRectangleSelectMode();
	void StartRectangleSelect(const FVector2D& Start, bool IsPositive);
	void EndRectangleSelect(bool Valid);
	void ExitRectangleSelectMode();

	void StartSetItemMode();
	void ExitSetItemMode(bool RestoreEquipment);

	void StartSetAnchorMode();
	void ExitSetAnchorMode(bool RestoreEquipment);

	void StartSelectMode();
	void ExitSelectMode(bool RestoreEquipment);

	void StartCopyMode();
	void ExitCopyMode(bool RestoreEquipment);

	void SetLeftMousePressed(bool Pressed);
	void SetRightMousePressed(bool Pressed);
	
	virtual void Tick(float dt);
	virtual void TickSelect(float dt);
	virtual void TickSetItem(float dt);
	virtual void TickCopy(float dt);
	virtual void TickFly(float dt);

	void onBuildFinish();
	void onSetRepeat();

	void ExecCallMenu();

	void UnequipBuilder();
	void EquipBuilder();
	void PopFGUI();
	void CheckFlying();
	bool CheckAnchor(bool Warn);
	AFGBuildable* AcquireBuildable(const FHitResult& Hit);

	FHitResult GetCopyHitResult();
	FHitResult GetSelectHitResult();
	FHitResult GetMouseCursorHitResult(bool RequireBuildable);
	void SetFocusBuilding(AFGBuildable* Buildable);
	void ClearFocusBuilding();
	void ChangeConnectSelectMode();

	void onSwitchFlying();
	void onSwitchFog();
	void onSwitchGodlike();
	void onSwitchThirdPersonView();
	void onSwitchRecycleMaterials();

	AFGCharacterPlayer* GetPlayer();
	TWeakObjectPtr<AFGCharacterPlayer> CurrentPlayer = nullptr;

	UWorld* World = nullptr;
	AFGWorldSettings* WorldSettings = nullptr;
	AFGPlayerController* FGController = nullptr;
	AFGResourceSinkSubsystem* ResourceSinkSubsystem = nullptr;
	AFGBuildGun* FGBuildGun = nullptr;
	UFGGameUI* FGUI = nullptr;
	AFGHUD* HUD = nullptr;

	UPROPERTY()
	UFSInput* FSInput = nullptr;

	UPROPERTY(SaveGame)
	UFSEtc* Etc = nullptr;

	UPROPERTY(SaveGame)
	UFSDesignMenu* DesignRoot = nullptr;

	UPROPERTY(SaveGame)
	FSInventory Inventory;

	UFSkylineUI* SkylineUI = nullptr;
	AFSBuilder* Builder = nullptr;
	UFSSelection* Select = nullptr;

	FSState State = FSState::Close;
	UFSDesign* Design = nullptr;
	FHitResult CurrentHitResult;
	TWeakObjectPtr<AFGBuildable> CurrentFocusBuilding = nullptr;
	FSTime Timer;
	FSRepeat Repeat;

	FCriticalSection Mutex;

	const float DistanceMax = 100000.0f;

	const float ShortPressLeftCtrlTimeLimit = 0.5f;
	const float ShortPressMouseRightTimeLimit = 0.2f;

	float LastLeftCtrlPressed = 0.0f;
	float LastRightMouseDown = 0.0f;

	FVector2D SelectStart;
	bool RectangleSelectMode = false;
	bool RectangleSelecting = false;
	bool RectanglePositive = false;
	bool LastShowMouseCursor = false;

	bool LeftCtrlPressed = false;
	bool LeftMousePressed = false;
	bool RightMousePressed = false;
	bool CallMenuPressed = false;

	bool ConnectSelectMode = false;
	bool SelectWaitingResult = false;
	bool CopyWaitingResult = false;
	bool FullPreview = false;

	bool TurboSprint = false;

	bool SelectOne = false;
	bool BuildOne = false;
	bool CopyPosition = false;
	bool CopyConsole = false;
	bool BuildRepeatCallBack = false;
	bool RecycleMaterials;
};
