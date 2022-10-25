// ILikeBanas


#include "FactorySkyline/FSGameWorldModule.h"

UFSGameWorldModule::UFSGameWorldModule()
{
	bRootModule = true;
	Log("Init Game World Module");
	ModSubsystems.Add(AFSkyline::StaticClass());
}
