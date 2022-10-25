// ILikeBanas


#include "FSConveyorSmartSplitterOperator.h"
#include "Buildables/FGBuildableSplitterSmart.h"


void UFSConveyorSmartSplitterOperator::ApplySettingsTo(AFGBuildable* Buildable)
{
	Super::ApplySettingsTo(Buildable);

	AFGBuildableSplitterSmart* SourceSplitter = Cast<AFGBuildableSplitterSmart>(Source);
	AFGBuildableSplitterSmart* TargetSplitter = Cast<AFGBuildableSplitterSmart>(Buildable);
	/*
	TargetSplitter->RemoveSortRuleAt(0);

	for (int i = 0; i < SourceSplitter->GetNumSortRules(); i++) {
		TargetSplitter->AddSortRule(SourceSplitter->GetSortRuleAt(i));
	}*/
	
	TargetSplitter->mSortRules = SourceSplitter->mSortRules;
	TargetSplitter->mMaxNumSortRules = SourceSplitter->mMaxNumSortRules;
	TargetSplitter->mLastItem = SourceSplitter->mLastItem;
	TargetSplitter->mItemToLastOutputMap = SourceSplitter->mItemToLastOutputMap;
	TargetSplitter->mLastOutputIndex = SourceSplitter->mLastOutputIndex;
}
