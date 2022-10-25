// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSMenuEntryBase.h"
#include "Blueprint/UserWidget.h"
#include "FSMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFSMenuWidget(const FObjectInitializer& ObjectInitializer);

	UObject* SkylineUI = nullptr;

	UObject* ParentItem = nullptr;

	TSet<UFSMenuEntryBase*> EntrySet;

	float LastY;

	bool DragFold = false;
	
	void Load(UObject* Item);

	void Unload();

	virtual void SetPosition(FVector2D Size);

	virtual void SetSize(FVector2D Size);

	void StartAnimation();

	void TickAnimation(float dt);

	virtual void NativeTick(const FGeometry& Geometry, float dt) override;

	bool IsPlayingAnimation;
	float AnimationTimeCountDown;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);

	UFUNCTION()
	void FSDragCancel(UDragDropOperation* Operation);
};
