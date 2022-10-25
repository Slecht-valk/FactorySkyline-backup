// ILikeBanas


#include "FSResourceIcon.h"


UFSResourceIcon::UFSResourceIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UFSResourceIcon::Load(TSubclassOf<UFGItemDescriptor> ItemClass, int Count)
{
	this->Icon->SetBrushFromTexture(UFGItemDescriptor::GetSmallIcon(ItemClass));
	this->Count->SetText(FText::FromString(FString::FromInt(Count)));
}
