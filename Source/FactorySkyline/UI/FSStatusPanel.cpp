// ILikeBanas


#include "FSStatusPanel.h"
#include "FactorySkyline/FSkyline.h"

void UFSStatusPanel::onCheckBoxChanged(bool IsChecked)
{
	if (IsChecked) this->Expand();
	else this->Fold();
}

void UFSStatusPanel::Init()
{
	FScriptDelegate Func;
	Func.BindUFunction(this, FName("onCheckBoxChanged"));
	this->FoldPanel->FoldBox->OnCheckStateChanged.Add(Func);

	this->FoldPanel->Title->SetText(FText::FromString(FString(TEXT("Building Status"))));

	this->FoldPanel->FoldBox->SetCheckedState(ECheckBoxState::Checked);
	this->onCheckBoxChanged(true);
}

void UFSStatusPanel::Load(UFSDesign* Design)
{
	this->Design = Design;
	AFSkyline* FSkyline = AFSkyline::Get(this);
	FSkyline->SplineHologramFactory->Load();

	Mapping.Reset();
	for (TWeakObjectPtr<AFGBuildable> Ptr : Design->BuildableSet) {
		AFGBuildable* Buildable = Ptr.Get();
		if (Buildable) {
			int* Count = Mapping.Find(Buildable->GetClass());
			if (Count) (*Count)++;
			else Mapping.Add(Buildable->GetClass(), 1);
		}
	}

	EntryList.Reset();
	for (TPair<TSubclassOf< class AFGBuildable >, int> Pair : Mapping) {
		EntryList.Add(FSEntry(Pair.Key, Pair.Value));
	}
	EntryList.Sort();

	int32 Children = RootBox->GetChildrenCount();
	for (int32 ChildIndex = Children-1; ChildIndex >=0; ChildIndex--)
		RootBox->RemoveChildAt(ChildIndex);

	for (FSEntry& Entry : EntryList) {
		UTexture2D* Texture = UFGItemDescriptor::GetSmallIcon(UFGRecipe::GetDescriptorForRecipe(AFSkyline::Get(this)->SplineHologramFactory->GetRecipeFromClass(Entry.BuildableClass)));
		if (Texture) {
			TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_StatusEntry.Widget_StatusEntry_C"));
			UFSStatusEntry* Widget = CreateWidget<UFSStatusEntry>(this, WidgetClass);
			Widget->Init(Design, Entry.BuildableClass, Entry.Count, Texture);
			Widget->SkylineUI = FSkyline->SkylineUI;
			RootBox->AddChild(Widget);
		}
	}

	this->FoldPanel->FoldBox->SetCheckedState(ECheckBoxState::Checked);
	this->onCheckBoxChanged(true);
}

void UFSStatusPanel::Fold()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::Collapsed);
	this->RootBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UFSStatusPanel::Expand()
{
	this->FoldPanel->Dark->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	this->RootBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
