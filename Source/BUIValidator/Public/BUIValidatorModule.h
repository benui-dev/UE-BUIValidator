#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Tickable.h"

class FBUIValidatorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool SupportsDynamicReloading() override { return true; }

protected:
	bool HandleSettingsSaved();

};
