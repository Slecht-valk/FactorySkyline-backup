// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FSSyncWork.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYSKYLINE_API UFSSyncWork : public UObject
{
	GENERATED_BODY()
public:
	virtual void Load();
	virtual void Unload();
	virtual bool DoWork(float TimeLimit);

	virtual int GetTotal();
	virtual int GetCurrent();
};
