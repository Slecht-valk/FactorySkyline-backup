// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "FactorySkyline/FSInventory.h"
#include "FactorySkyline/UI/FSInventoryEntry.h"
#include "FSInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSInventoryWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* RootPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* RootBox;

	void NativeTick(const FGeometry& Geometry, float dt);

	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void FSMouseWheel(float DeltaY);

	void Init(FSInventory* Inventory);
	void Toggle();

	UFSInventoryEntry* AccquireEntry(TSubclassOf<UFGItemDescriptor> ItemClass, int Count);

	UPROPERTY()
	TMap<TSubclassOf<UFGItemDescriptor>, UFSInventoryEntry*> EntryMapping;

	FSInventory* Inventory;
	TMap<TSubclassOf<UFGItemDescriptor>, UFSInventoryEntry*> EntryCache;
	UObject* SkylineUI;
};
