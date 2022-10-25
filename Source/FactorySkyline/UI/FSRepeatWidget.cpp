// ILikeBanas


#include "FSRepeatWidget.h"
#include "FactorySkyline/FSkyline.h"

UFSRepeatWidget::UFSRepeatWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSRepeatWidget::onBuildOne()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (!Skyline->Builder->IsBuilding && !Skyline->FSCtrl->CopyWaitingResult) {
		int Count = GetCount() - 1;
		;
		Skyline->Builder->LastShow = true;
		Skyline->Builder->LastValid = true;
		if (Skyline->Builder->Build(&Skyline->FSCtrl->Repeat)) {
			SetCount(Count > 0 ? Count : 0);
			Skyline->FSCtrl->BuildRepeatCallBack = true;
			Skyline->FSCtrl->CopyWaitingResult = true;
			Skyline->SkylineUI->MessageBox->SetVisibility(ESlateVisibility::Visible);
			Skyline->FSCtrl->Repeat.Target = Skyline->FSCtrl->Repeat.Next();
		}
		else {
			BuildAll = false;
			//Skyline->Builder->Update(&Skyline->FSCtrl->Repeat);
		}

	}
}

void UFSRepeatWidget::onBuildAll()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	if (!Skyline->Builder->IsBuilding && !Skyline->FSCtrl->CopyWaitingResult && GetCount() > 0) {
		BuildAll = true;
		onBuildOne();
	}
}

void UFSRepeatWidget::OnEditableTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	int Value = FCString::Atoi(*Text.ToString());
	if (Value < 0) Value = 0;
	FText NewText = FText::FromString(FString::Printf(TEXT("%d"), Value));
	Count->SetText(NewText);
}

void UFSRepeatWidget::Init()
{
	FScriptDelegate Func;
	Func.BindUFunction(this, FName("onBuildOne"));
	this->BuildOneButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("onBuildAll"));
	this->BuildAllButton->OnClicked.Add(Func);

	Func.BindUFunction(this, FName("OnEditableTextCommitted"));
	this->Count->OnTextCommitted.Add(Func);

	PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	IsPlayingEnterAnimation = false;
}

void UFSRepeatWidget::Load()
{
	SetCount(0);
	BuildAll = false;
}

void UFSRepeatWidget::CallBack()
{
	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->FSCtrl->BuildRepeatCallBack = false;
	if (BuildAll) {
		if (GetCount() > 0) {
			onBuildOne();
			return;
		}
		else BuildAll = false;
	}
	//Skyline->Builder->Update(&Skyline->FSCtrl->Repeat);
}

void UFSRepeatWidget::NativeTick(const FGeometry& Geometry, float dt)
{
	Super::NativeTick(Geometry, dt);
	if (IsPlayingEnterAnimation) {
		TickEnterAnimation(dt);
	}
}

void UFSRepeatWidget::StartEnterAnimation()
{
	PanelSlot->SetSize(FVector2D(300.0f, 60.0f));
	RepeatBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Title->SetVisibility(ESlateVisibility::Collapsed);
	this->SetVisibility(ESlateVisibility::Visible);
	this->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	IsPlayingEnterAnimation = true;
	AnimationTimeCountDown = 0.5;
}

void UFSRepeatWidget::TickEnterAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		this->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		IsPlayingEnterAnimation = false;
		return;
	}
	this->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f - AnimationTimeCountDown / 0.5f));
}

int UFSRepeatWidget::GetCount()
{
	return FCString::Atoi(*Count->GetText().ToString());
}

void UFSRepeatWidget::SetCount(int Count)
{
	FText NewText = FText::FromString(FString::Printf(TEXT("%d"), Count));
	this->Count->SetText(NewText);
}