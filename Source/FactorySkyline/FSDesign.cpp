// ILikeBanas


#include "FSDesign.h"
#include "FSkyline.h"
#include "FSController.h"
#include "Buildables/FGBuildable.h"

bool UFSDesign::ShouldSave_Implementation() const 
{
	AFSkyline* FSkyline = AFSkyline::Get(this->GetWorld());
	return FSkyline && FSkyline->FSCtrl && FSkyline->FSCtrl->Etc ? !FSkyline->FSCtrl->Etc->GetBool("DontSaveAnything") : false;
}

void UFSDesign::PreSaveGame_Implementation(int32 saveVersion, int32 gameVersion)
{
	AnchorSave = Anchor.Get();
	Set.Empty();
	for (TWeakObjectPtr<AFGBuildable>& Ptr : BuildableSet) {
		if (Ptr.Get()) Set.Add(Ptr.Get());
	}
	Mark.Empty();
	for (TPair<TWeakObjectPtr<AFGBuildable>,int>& Pair : BuildableMark) {
		if (Pair.Key.Get()) Mark.Add(Pair.Key.Get(), Pair.Value);
	}
}

void UFSDesign::PostSaveGame_Implementation(int32 saveVersion, int32 gameVersion)
{
	AnchorSave = nullptr;
	Set.Empty();
	Mark.Empty();
}

void UFSDesign::PostLoadGame_Implementation(int32 saveVersion, int32 gameVersion)
{
	Anchor = AnchorSave;
	BuildableSet.Empty();
	BuildableMark.Empty();
	for (AFGBuildable*& Ptr : Set) {
		BuildableSet.Add(Ptr);
	}
	for (TPair<AFGBuildable*, int>& Pair : Mark) {
		BuildableMark.Add(Pair.Key, Pair.Value);
	}
	AnchorSave = nullptr;
	Set.Empty();
	Mark.Empty();
}

bool UFSDesign::IsElementSelected(AFGBuildable* Buildable)
{
	if (!Buildable) return false;
	TWeakObjectPtr<AFGBuildable>* Result = BuildableSet.Find(Buildable);
	return Result && Result->Get();
}

void UFSDesign::AddElement(AFGBuildable* Buildable)
{
	BuildableSet.Add(Buildable);
	BuildableArray.Add(Buildable);
}

void UFSDesign::RemoveElement(AFGBuildable* Buildable)
{
	BuildableSet.Remove(Buildable);
	BuildableArray.Remove(Buildable);

}

void UFSDesign::SetElementMark(AFGBuildable* Buildable, bool MarkParam)
{
	//BuildableMark[Buildable] = Mark;
}

void UFSDesign::ClearAll()
{
	this->BuildableSet.Empty();
	this->BuildableMark.Empty();
	this->Anchor = nullptr;
}

void UFSDesign::DismantleAll()
{
	AFSkyline* FSkyline = AFSkyline::Get(this->GetWorld());
	TArray<AFGBuildable*> List;
	for (TWeakObjectPtr<AFGBuildable> Buildable : BuildableSet) if (Buildable.Get()) {
		FSkyline->FSCtrl->Inventory.AddResourceCheckRecipe(Buildable.Get());

		//Buildable.Get()->SetHiddenIngameAndHideInstancedMeshes(true);

		for (UActorComponent* ActorComponent : Buildable.Get()->GetComponents()) {
			ActorComponent->UnregisterComponent();
		}
		List.Add(Buildable.Get());
	}
	for (int i = 0; i < List.Num(); i++) {
		List[i]->TurnOffAndDestroy();
	}

	ClearAll();
}

void UFSDesign::RecheckNullptr()
{
	TSet< TWeakObjectPtr<AFGBuildable> > SetVar;
	TMap< TWeakObjectPtr<AFGBuildable>, int> MarkVar;

	for (TWeakObjectPtr<AFGBuildable>& Ptr : this->BuildableSet)
		if (Ptr.Get()) SetVar.Add(Ptr);
	for (TPair<TWeakObjectPtr<AFGBuildable>, int>& Pair : this->BuildableMark)
		if (Pair.Key.Get()) MarkVar.Add(Pair);

	this->BuildableSet = SetVar;
	this->BuildableMark = MarkVar;
}