// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "UI/FSMenuWidget.h"
#include "UI/FSMenuEntryBase.h"
#include "UI/FSButtonWidget.h"
#include "UObject/NoExportTypes.h"
#include "FSDesign.h"
#include "FSMenuItem.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSMenuItem : public UObject, public IFGSaveInterface
{
	GENERATED_BODY()
public:
	
	
	UPROPERTY(SaveGame)
	TArray<UFSMenuItem*> Elements;

	UPROPERTY()
	UFSMenuEntryBase* CurrentEntry = nullptr;

	UPROPERTY()
	UFSMenuWidget* CurrentMenuWidget = nullptr;


	virtual UFSMenuEntryBase* AccquireEntry();
	virtual UFSMenuWidget* AccquireMenuWidget();

	virtual void ReleaseEntry();
	virtual void ReleaseMenuWidget();

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual UFSMenuWidget* ConstructMenuWidget();

	virtual void InitEntryStyle(UFSMenuEntryBase* Child);
	virtual void InitWidgetStyle();

	virtual void InitItem();
	virtual void FSClick();
	virtual void ExecClick();
	virtual void Expand();
	virtual void Unexpand();

	virtual void AddChildAt(UFSMenuItem* Child, int32 index);
	virtual void RemoveChild(UFSMenuItem* Child);
	void AddChild(UFSMenuItem* Child);
	void AddChildAfter(UFSMenuItem* Child, UFSMenuItem* After);

	UObject* SkylineUI = nullptr;

	UFSMenuItem* CurrentExpandedItem = nullptr;
	UFSMenuItem* ParentItem = nullptr;

	bool IsMultiSelected = false;

	template <typename T>
	T* CreateUserWidget(const TCHAR* Class);
};

UCLASS()
class FACTORYSKYLINE_API UFSSingleMenu : public UFSMenuItem
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* AccquireEntry();

	virtual void FSClick();

};

UCLASS()
class FACTORYSKYLINE_API UFSSwitchMenu : public UFSSingleMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitEntry(UFSButtonWidget* Entry) {}

	virtual void ExecClick();
	virtual void InitItem();
	virtual void onSwitcherChanged() {}

	FName AttributeName;
};

UCLASS()
class FACTORYSKYLINE_API UFSListMenu : public UFSMenuItem
{
	GENERATED_BODY()
public:

	const float EntryHeight = 40.0f;

	float Height = 0.0f;

	float CurrentY = 0.0f;
	float MoveToY = 0.0f;

	float WidgetWidth = 0.0f;
	float WidgetHeight = 0.0f;

	bool DirtyWidget = true;

	virtual void AddChildAt(UFSMenuItem* Child, int32 index);
	virtual void RemoveChild(UFSMenuItem* Child);

	virtual UFSMenuEntryBase* AccquireEntry();
	virtual UFSMenuWidget* AccquireMenuWidget();
	virtual UFSMenuWidget* ConstructMenuWidget();

	virtual void InitEntryStyle(UFSMenuEntryBase* Child);
	virtual void InitWidgetStyle();
	virtual void FSClick();
	virtual void ExecClick();
	virtual void MoveTo(UFSMenuItem* Child);

	virtual void Expand();
	virtual void Expand(FVector2D Position);

};

UCLASS()
class FACTORYSKYLINE_API UFSDesignMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual bool ShouldSave_Implementation() const override;

	static UFSDesignMenu* CreateDesignMenu(UObject* WorldContext, bool WithDesign);
	static UFSDesignMenu* CopyFrom(UObject* WorldContext, UFSDesignMenu* Source, bool WithDesign);

	UPROPERTY(SaveGame)
	UFSDesign* Design = nullptr;

	UPROPERTY(SaveGame)
	FText FileName;

	UPROPERTY(SaveGame)
	bool IsFolder;

	bool Folded = false;

	virtual void LoadInit(UObject* SkylineUIParam, UFSDesignMenu* Parent);
	
	virtual UFSMenuEntryBase* AccquireEntry();
	virtual UFSMenuWidget* AccquireMenuWidget();
	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void ReleaseEntry();
	virtual void ReleaseMenuWidget();

	virtual void InitWidgetStyle();
	//virtual void FSClick();
	virtual void ExecClick();
	virtual void Expand();
	virtual void Expand(FVector2D Position);
	virtual void Unexpand();

	virtual void Fold();
	virtual void Unfold();
};

UCLASS()
class FACTORYSKYLINE_API UFSGettingStartMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSFastConstructMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSNewDesignMenu : public UFSSingleMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void ExecClick();
};

UCLASS()
class FACTORYSKYLINE_API UFSNewFolderMenu : public UFSSingleMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void ExecClick();
};

UCLASS()
class FACTORYSKYLINE_API UFSKeySettingMenu : public UFSSingleMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void ExecClick();
};

UCLASS()
class FACTORYSKYLINE_API UFSInventoryMenu : public UFSSingleMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void ExecClick();
};

UCLASS()
class FACTORYSKYLINE_API UFSTookitMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSFoundationIConstructMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
};

UCLASS()
class FACTORYSKYLINE_API UFSFoundationIIConstructMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
};

UCLASS()
class FACTORYSKYLINE_API UFSInventoryOptionsMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSRecycleMaterialsMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
	virtual void onSwitcherChanged();
};

UCLASS()
class FACTORYSKYLINE_API UFSTookitFlyMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSDoubleSpaceFlyingMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
	virtual void onSwitcherChanged();
};

UCLASS()
class FACTORYSKYLINE_API UFSDoubleShiftSprintMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSFogControlMenu : public UFSListMenu
{
	GENERATED_BODY()
public:

	virtual UFSMenuEntryBase* ConstructEntry();
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSDisableFogMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
	virtual void onSwitcherChanged();
};

UCLASS()
class FACTORYSKYLINE_API UFSDisableHighSpaceFogMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSGodlikeMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
	virtual void onSwitcherChanged();
};

UCLASS()
class FACTORYSKYLINE_API UFSThirdPersonViewMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
	virtual void onSwitcherChanged();
};

UCLASS()
class FACTORYSKYLINE_API UFSInfiniteAmmoMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
};

UCLASS()
class FACTORYSKYLINE_API UFSDontSaveAnythingMenu : public UFSSwitchMenu
{
	GENERATED_BODY()
public:

	virtual void InitEntry(UFSButtonWidget* Entry);
	virtual void InitItem();
};