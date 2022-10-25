// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "FSAdaptiveUtil.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSAdaptiveUtil : public UObject
{
	GENERATED_BODY()
public:

	int GameVersion;

	void Init();
	int GetGameVersion();

	UFGFactoryConnectionComponent* GetConveyorConnection(AFGBuildableConveyorBase* Conveyor, int index);
	TArray< FSplinePointData >* GetConveyorBeltSplineData(AFGBuildableConveyorBelt* Conveyor);
	void CopyConveyorLiftAttribute(AFGBuildableConveyorLift* Source, AFGBuildableConveyorLift* Target);
};
