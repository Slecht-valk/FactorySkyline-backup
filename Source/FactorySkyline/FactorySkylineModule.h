#pragma once

#include "Modules/ModuleManager.h"
#include "FSkyline.h"

class FFactorySkylineModule : public FDefaultGameModuleImpl {
public:
	virtual void StartupModule() override;

	virtual bool IsGameModule() const override { return true; }

private:

	
};