// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildable.h"
#include "FSDesign.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSDesign : public UObject, public IFGSaveInterface
{
	GENERATED_BODY()
public:

	virtual bool ShouldSave_Implementation() const override;
	void PreSaveGame_Implementation(int32 saveVersion, int32 gameVersion) override;
	void PostSaveGame_Implementation(int32 saveVersion, int32 gameVersion) override;
	void PostLoadGame_Implementation(int32 saveVersion, int32 gameVersion) override;

	bool IsElementSelected(AFGBuildable* Buildable);
	void AddElement(AFGBuildable* Buildable);
	void RemoveElement(AFGBuildable* Buildable);
	void SetElementMark(AFGBuildable* Buildable, bool MarkParam);

	void ClearAll();
	void DismantleAll();

	void RecheckNullptr();

	TSet< TWeakObjectPtr<AFGBuildable> > BuildableSet;
	TMap< TWeakObjectPtr<AFGBuildable>, int> BuildableMark;
	TWeakObjectPtr<AFGBuildable> Anchor = nullptr;
	TArray<AFGBuildable*> BuildableArray;

	UPROPERTY(SaveGame)
	TSet<AFGBuildable*> Set;

	UPROPERTY(SaveGame)
	TMap<AFGBuildable*, int> Mark;

	UPROPERTY(SaveGame)
	AFGBuildable* AnchorSave = nullptr;
	
	UTextBlock* SetItemFeedback = nullptr;
};
