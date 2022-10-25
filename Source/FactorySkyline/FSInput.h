// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FGPlayerController.h"
#include "FSBuildableService.h"
#include "UI/FSKeySettingEntry.h"
#include "FSInput.generated.h"

DECLARE_DELEGATE(FOnKeyAction);

enum FSInputEvent
{
	FS_Pressed = 0,
	FS_Released = 1,
	FS_Repeat = 2,
};

struct FSKey
{
	FSKey() {}

	FSKey(FKey Key, bool Ctrl = false, bool Alt = false, bool Shift = false, bool Double = false)
	{
		this->Key = Key;
		this->Ctrl = Ctrl && Key != EKeys::LeftControl && Key != EKeys::RightControl;
		this->Alt = Alt && Key != EKeys::LeftAlt && Key != EKeys::RightAlt;
		this->Shift = Shift && Key != EKeys::LeftShift && Key != EKeys::RightShift;
		this->Double = Double;
	}

	FKey Key = EKeys::AnyKey;
	bool Ctrl = false;
	bool Alt = false;
	bool Shift = false;
	bool Double = false;

	FString GetDesc() const
	{
		FString Result = TEXT("");
		if (Double) Result.Append(TEXT("Double "));
		if (Ctrl && Key != EKeys::LeftControl && Key != EKeys::RightControl) Result.Append(TEXT("Ctrl + "));
		if (Alt && Key != EKeys::LeftAlt && Key != EKeys::RightAlt) Result.Append(TEXT("Alt + "));
		if (Shift && Key != EKeys::LeftShift && Key != EKeys::RightShift) Result.Append(TEXT("Shift + "));
		Result.Append(Key.ToString());
		return Result;
	}

	FString ToString() const
	{
		FString Result = TEXT("");
		if (Double) Result.Append(TEXT("Double\n"));
		if (Ctrl) Result.Append(TEXT("Ctrl\n"));
		if (Alt) Result.Append(TEXT("Alt\n"));
		if (Shift) Result.Append(TEXT("Shift\n"));
		Result.Append(Key.ToString());
		return Result;
	}

	static FSKey FromString(FString String) {
		FSKey Result;
		FString Left;
		FString Right;
		while (String.Split(TEXT("\n"), &Left, &Right)) {
			if (Left.Equals(TEXT("Double"), ESearchCase::IgnoreCase)) Result.Double = true;
			if (Left.Equals(TEXT("Ctrl"), ESearchCase::IgnoreCase)) Result.Ctrl = true;
			if (Left.Equals(TEXT("Alt"), ESearchCase::IgnoreCase)) Result.Alt = true;
			if (Left.Equals(TEXT("Shift"), ESearchCase::IgnoreCase)) Result.Shift = true;
			String = Right;
		}
		Result.Key = FKey(*String);
		return Result;
	}
};

struct FSActionKey
{
	FSActionKey(const FName& Action, const FSKey& Key)
	{
		this->Action = Action;
		this->Key = Key;
		LastDown = false;
		LastPress.Start();
	}

	FSActionKey(const FName& Action, const FSKey& Key, const FText& Desc) : FSActionKey(Action, Key)
	{
		this->Desc = Desc;
	}

	FName Action;
	FText Desc;
	FSKey Key;
	FSKey PreSet;
	bool LastDown;
	bool KeyDown;
	FSTime LastPress;
	bool Configable;

	FOnKeyAction OnKeyPressed;
	FOnKeyAction OnKeyReleased;
	FOnKeyAction OnKeyRepeat;
};

class FScaner
{
public:
	void Init(AFGPlayerController* FGControllerParam);
	void Load(UFSKeySettingEntry* Entry);
	void Unload(UFSKeySettingEntry* Entry);
	void Tick();

	void AddKey(const FKey& Key);
	void Update(const FSKey& Key);

	const double DoubleClickTime = 0.5;

	AFGPlayerController* FGController = nullptr;
	UFSKeySettingEntry* CurrentEntry = nullptr;

	TArray<FSActionKey> ScaningList;

};

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSInput : public UObject
{
	GENERATED_BODY()
public:

	void Init();

	void Tick();

	void InitKey(const FName& Action, const FSKey& Key, const FText& Desc, bool Configable);

	FSActionKey* BindKey(const FName& Action, const FSKey& Key);
	
	template<class T>
	void BindDelegate(const FName& Action, FSInputEvent Event, T* InObject, void(T::*InFunc)());

	bool IsKeyDown(const FName& Action);

	void InitWidget();
	void UpdateCurrent();
	void UpdatePreSet();
	void ApplySetting();

	const double DoubleClickTime = 0.5;

	AFGPlayerController* FGController = nullptr;
	
	FScaner Scaner;
	TMap<FName, FSActionKey> KeyMapping;
	TArray<FName> ConfigList;
	bool IsActive = false;
	bool IsScaning = false;

};

template<class T>
inline void UFSInput::BindDelegate(const FName& Action, FSInputEvent Event, T* InObject, void(T::*InFunc)())
{
	FSActionKey* ActionKey = KeyMapping.Find(Action);
	if (ActionKey) {
		if (Event == FS_Pressed) ActionKey->OnKeyPressed.BindUObject(InObject, InFunc);
		if (Event == FS_Released) ActionKey->OnKeyReleased.BindUObject(InObject, InFunc);
		if (Event == FS_Repeat) ActionKey->OnKeyRepeat.BindUObject(InObject, InFunc);
	}
}
