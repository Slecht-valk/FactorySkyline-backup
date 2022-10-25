// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FSMenuEntryBase.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSMenuEntryBase : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSMenuEntryBase(const FObjectInitializer& ObjectInitializer);

	UObject* SkylineUI = nullptr;
	UObject* Item = nullptr;


	virtual void SetSelectedView();

	virtual void SetPosition(float x, float y);

	virtual float GetEntryHeight();
	virtual void SetEntryHeight(float Height);
	float Height;

	bool IsEnabled = false;
	bool IsEditable = false;

};
