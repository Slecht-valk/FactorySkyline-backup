// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSFoldWidgetBase.h"
#include "FactorySkyline/FSDesign.h"
#include "Blueprint/UserWidget.h"
#include "Buildables/FGBuildable.h"
#include "FSAdvancedCopyPanel.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSAdvancedCopyPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	UFSAdvancedCopyPanel(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* BuildButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SetRepeatButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ClipboardButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SetCircleButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UFSFoldWidgetBase* FoldPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* BuildStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* BuildPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* SetRepeatPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* ClipboardPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCanvasPanel* SetCirclePanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* VectorX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* VectorY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* VectorZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* RotationX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* RotationY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* RotationZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* RotationW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableText* RotationAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* VectorXBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* VectorYBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* VectorZBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RotationXBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RotationYBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RotationZBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RotationWBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* RotationAngleBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* CheckBoxVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* CheckBoxRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* CheckBoxCircle;

	TWeakObjectPtr<AFGBuildable> CircleCenter = nullptr;
	UFSDesign* Design = nullptr;

	UFUNCTION()
	void onCheckBoxChanged(bool IsChecked);
	UFUNCTION()
	void onCheckBoxChangedVector(bool IsChecked);
	UFUNCTION()
	void onCheckBoxChangedRotation(bool IsChecked);
	UFUNCTION()
	void onCheckBoxChangedCircle(bool IsChecked);

	UFUNCTION()
	void OnEditableTextCommittedVectorX(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedVectorY(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedVectorZ(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedRotationX(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedRotationY(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedRotationZ(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedRotationW(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnEditableTextCommittedRotationAngle(const FText& Text, ETextCommit::Type CommitMethod);

	FString Format(double Value);

	UFUNCTION()
	void onBuild();

	UFUNCTION()
	void onSetRepeat();

	UFUNCTION()
	void onSetCircle();

	UFUNCTION()
	void onClipboard();

	void SetCircleFeedback();
	void UpdateCircle();

	void Init();
	void Load(UFSDesign* Design);

	void UpdateTransform();
	FTransform GetTransform();
	void SetTransform(const FTransform& Transform);

	void Fold();
	void Expand();
};
