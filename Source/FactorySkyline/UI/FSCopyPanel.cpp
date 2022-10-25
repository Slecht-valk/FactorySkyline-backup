// ILikeBanas


#include "FactorySkyline/UI/FSCopyPanel.h"
#include "FactorySkyline/FSkyline.h"


UFSCopyPanel::UFSCopyPanel(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSCopyPanel::onCheckBoxChanged(bool IsChecked)
{
	if (IsChecked) this->Expand();
	else this->Fold();
}

void UFSCopyPanel::onFullPreviewChanged(bool IsChecked)
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->FullPreview = IsChecked;
}

void UFSCopyPanel::onSetItem()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->StartSetAnchorMode();
}

void UFSCopyPanel::onStartCopy()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (Skyline->FSCtrl->CheckAnchor(true)) {
		Skyline->FSCtrl->StartCopyMode();
	}
}

void UFSCopyPanel::Init()
{
	FScriptDelegate Func1;
	Func1.BindUFunction(this, FName("onCheckBoxChanged"));
	this->FoldPanel->FoldBox->OnCheckStateChanged.Add(Func1);

	Func1.BindUFunction(this, FName("onFullPreviewChanged"));
	this->CheckBoxFullPreview->OnCheckStateChanged.Add(Func1);

	Func1.BindUFunction(this, FName("onSetItem"));
	this->SetItemButton->OnClicked.Add(Func1);

	Func1.BindUFunction(this, FName("onStartCopy"));
	this->StartCopyButton->OnClicked.Add(Func1);

	this->FoldPanel->Title->SetText(FText::FromString(FString(TEXT("Basic Copy"))));
	this->SetItemStatus->SetVisibility(ESlateVisibility::Collapsed);

	this->FoldPanel->FoldBox->SetCheckedState(ECheckBoxState::Unchecked);
	this->onCheckBoxChanged(false);

	this->CheckBoxFullPreview->SetCheckedState(ECheckBoxState::Unchecked);
	this->onFullPreviewChanged(false);
}

void UFSCopyPanel::Load(UFSDesign* Design)
{
	this->Design = Design;
	this->Design->SetItemFeedback = this->SetItemStatus;
	 AFSkyline::Get(this)->FSCtrl->CheckAnchor(false);
}

void UFSCopyPanel::Fold()
{
}

void UFSCopyPanel::Expand()
{
}
