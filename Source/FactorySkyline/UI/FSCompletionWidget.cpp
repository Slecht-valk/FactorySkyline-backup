// ILikeBanas


#include "FSCompletionWidget.h"
#include "FactorySkyline/FSkyline.h"


UFSCompletionWidget::UFSCompletionWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSCompletionWidget::onUndo()
{
	this->Target->Design->DismantleAll();
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->Target = nullptr;
}

void UFSCompletionWidget::onOk()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->Target = nullptr;
}

void UFSCompletionWidget::onSave()
{
	if (Source.Get()) {
		Source.Get()->ParentItem->AddChildAfter(Target, Source.Get());
	}
	else {
		AFSkyline* Skyline = AFSkyline::Get(this);
		Skyline->SkylineUI->DesignRoot->AddChild(Target);
	}
	this->SetVisibility(ESlateVisibility::Collapsed);
	this->Target = nullptr;
}

void UFSCompletionWidget::Init()
{
	FScriptDelegate Func1;
	Func1.BindUFunction(this, FName("onUndo"));
	this->UndoButton->OnClicked.Add(Func1);

	Func1.BindUFunction(this, FName("onOk"));
	this->OkButton->OnClicked.Add(Func1);

	Func1.BindUFunction(this, FName("onSave"));
	this->SaveButton->OnClicked.Add(Func1);

	PanelSlot = Cast<UCanvasPanelSlot>(Slot);
}

void UFSCompletionWidget::Load(TWeakObjectPtr<UFSDesignMenu> Source, UFSDesignMenu* Target)
{
	this->Source = Source;
	this->Target = Target;
}

void UFSCompletionWidget::NativeTick(const FGeometry& Geometry, float dt)
{
	Super::NativeTick(Geometry, dt);
	if (IsPlayingEnterAnimation) {
		TickEnterAnimation(dt);
	}
}

void UFSCompletionWidget::StartEnterAnimation()
{
	PanelSlot->SetPosition(FVector2D(-300.0f, 728.0f));
	PanelSlot->SetSize(FVector2D(300.0f, 30.0f));
	this->SetVisibility(ESlateVisibility::Visible);
	IsPlayingEnterAnimation = true;
	AnimationTimeCountDown = 0.5;
}

void UFSCompletionWidget::TickEnterAnimation(float dt)
{
	AnimationTimeCountDown -= dt;
	if (AnimationTimeCountDown < 1e-4) {
		PanelSlot->SetPosition(FVector2D(0.0f, 728.0f));
		PanelSlot->SetSize(FVector2D(300.0f, 90.0f));
		IsPlayingEnterAnimation = false;
		return;
	}
	PanelSlot->SetPosition(FVector2D(-480.0 * AnimationTimeCountDown / 0.5, 728.0f));
}