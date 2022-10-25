// ILikeBanas


#include "FSDeletePanel.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSMenuItem.h"
#include "FactorySkyline/UI/FSkylineUI.h"
#include "Buildables/FGBuildable.h"


UFSDeletePanel::UFSDeletePanel(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSDeletePanel::onCheckBoxChanged(bool IsChecked)
{
	if (IsChecked) this->Expand();
	else this->Fold();
}

void UFSDeletePanel::onClear()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->SetOpenState(true);

	this->Design->ClearAll();
}

void UFSDeletePanel::onDestroy()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->SetOpenState(true); 

	this->Design->DismantleAll();
}

void UFSDeletePanel::onRemove()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->SetOpenState(true);
	UFSDesignMenu* Item = Cast<UFSDesignMenu>(Skyline->SkylineUI->OperatingWidget->ParentItem);
	if (Item) {
		UFSDesignMenu* Parent = Cast<UFSDesignMenu>(Item->ParentItem);
		if (Parent) {
			if (Item->AccquireEntry() == Skyline->SkylineUI->CurrentHighLightButton)
				Skyline->SkylineUI->CurrentHighLightButton = nullptr;

			if (Parent->Folded) Item->Unfold();

			Item->Unexpand();

			Parent->RemoveChild(Item);
		}

		Skyline->SkylineUI->CompletionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UFSDeletePanel::Init()
{
	FScriptDelegate Func1;
	Func1.BindUFunction(this, FName("onCheckBoxChanged"));
	this->FoldPanel->FoldBox->OnCheckStateChanged.Add(Func1);

	Func1.BindUFunction(this, FName("onClear"));
	this->ClearButton->OnClicked.Add(Func1);

	Func1.BindUFunction(this, FName("onDestroy"));
	this->DestroyButton->OnClicked.Add(Func1);

	Func1.BindUFunction(this, FName("onRemove"));
	this->DeleteButton->OnClicked.Add(Func1);

	this->FoldPanel->Title->SetText(FText::FromString(FString(TEXT("Remove"))));
}

void UFSDeletePanel::Load(UFSDesign* Design)
{
	this->Design = Design;
	this->FoldPanel->FoldBox->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChanged(false);
}

void UFSDeletePanel::Fold()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::Collapsed);
	this->ClearPanel->SetVisibility(ESlateVisibility::Collapsed);
	this->DestroyPanel->SetVisibility(ESlateVisibility::Collapsed);
	this->DeletePanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UFSDeletePanel::Expand()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->ClearPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->DestroyPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->DeletePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
