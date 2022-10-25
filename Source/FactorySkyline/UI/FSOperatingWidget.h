// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "FactorySkyline/UI/FSMenuWidget.h"
#include "FactorySkyline/UI/FSStatusPanel.h"
#include "FactorySkyline/UI/FSSelectPanel.h"
#include "FactorySkyline/UI/FSCopyPanel.h"
#include "FactorySkyline/UI/FSDeletePanel.h"
#include "FactorySkyline/UI/FSAdvancedCopyPanel.h"
#include "FactorySkyline/FSDesign.h"
#include "FSOperatingWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSOperatingWidget : public UFSMenuWidget
{
	GENERATED_BODY()
public:

	UFSOperatingWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* Box1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* Box2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* Box3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Tab1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Tab2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Tab3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* MainPanel;

	void Init();
	void Load(UFSDesignMenu* Design);
	void Unload();

	UFUNCTION()
	void OnTab1Click();

	UFUNCTION()
	void OnTab2Click();

	UFUNCTION()
	void OnTab3Click();

	void NativeTick(const FGeometry& Geometry, float dt);

	FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	void FSMouseWheel(float DeltaY);

	int TabIndex = -1;
	FButtonStyle InStyle;
	FButtonStyle InStyleClick;
	UFSDesign* Design = nullptr;

	UVerticalBox* CurrentBox = nullptr;
	UFSSelectPanel* SelectPanel = nullptr;
	UFSDeletePanel* DeletePanel = nullptr;
	UFSCopyPanel* CopyPanel = nullptr;
	UFSAdvancedCopyPanel* AdvancedCopyPanel = nullptr;
	UFSStatusPanel* StatusPanel = nullptr;

	virtual float Refresh();
};
