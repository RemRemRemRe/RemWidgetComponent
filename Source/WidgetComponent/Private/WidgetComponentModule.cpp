// Copyright Epic Games, Inc. All Rights Reserved.

#include "WidgetComponentModule.h"

class FWidgetComponentModule : public IWidgetComponentModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FWidgetComponentModule, WidgetComponent)

void FWidgetComponentModule::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	IWidgetComponentModule::StartupModule();
}

void FWidgetComponentModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IWidgetComponentModule::ShutdownModule();
}