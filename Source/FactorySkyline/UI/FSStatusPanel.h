// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "FactorySkyline/UI/FSFoldWidgetBase.h"
#include "FactorySkyline/UI/FSStatusEntry.h"
#include "FactorySkyline/FSDesign.h"
#include "Blueprint/UserWidget.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableFactory.h"
#include "FSStatusPanel.generated.h"

struct FSEntry
{
	FSEntry(TSubclassOf< class AFGBuildable > BuildableClass, int Count)
	{
		this->BuildableClass = BuildableClass;
		this->Count = Count;
	}

	friend inline bool operator< (const FSEntry& A, const FSEntry& B)
	{
		if (A.BuildableClass->IsChildOf<AFGBuildableFactory>()) {
			if (B.BuildableClass->IsChildOf<AFGBuildableFactory>())
				return A.Count > B.Count;
			return true;
		}
		if (B.BuildableClass->IsChildOf<AFGBuildableFactory>())
			return false;
		return A.Count > B.Count;
	}

	TSubclassOf< class AFGBuildable > BuildableClass;
	int Count;
};

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSStatusPanel : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* RootBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UFSFoldWidgetBase* FoldPanel;

	UFUNCTION()
	void onCheckBoxChanged(bool IsChecked);

	void Init();
	void Load(UFSDesign* Design);

	void Fold();
	void Expand();

	UPROPERTY()
	TArray<UFSStatusEntry*> WidgetList;

	UFSDesign* Design = nullptr;

	TArray<FSEntry> EntryList;
	TMap<TSubclassOf< class AFGBuildable >, int> Mapping;

};
