// ILikeBanas


#include "FSPowerPoleOperator.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildablePowerPole.h"
#include "FGPowerConnectionComponent.h"


FSBuildableType UFSPowerPoleOperator::GetType() const
{
	return FSBuildableType::Power;
}

void UFSPowerPoleOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
	AFGBuildablePowerPole* PowerPole = Cast<AFGBuildablePowerPole>(Buildable);
	if (!PowerPole) return;

	UFGPowerConnectionComponent* ConnectionComponent = PowerPole->mPowerConnections[0];
	if (!ConnectionComponent) return;

	TArray<AFGBuildableWire*> ConnectionWires;
	ConnectionComponent->GetWires(ConnectionWires);

	for (AFGBuildableWire* TargetWire : ConnectionWires) {
		AFGBuildable* Wire = Cast<AFGBuildable>(TargetWire);
		if (Wire) List.Add(Wire);
	}
}