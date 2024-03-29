// Copyright Epic Games, Inc. All Rights Reserved.

#include "RemWidgetComponentModule.h"

class FRemWidgetComponentModule : public IRemWidgetComponentModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FRemWidgetComponentModule, RemWidgetComponent)

void FRemWidgetComponentModule::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	IRemWidgetComponentModule::StartupModule();
}

void FRemWidgetComponentModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IRemWidgetComponentModule::ShutdownModule();
}
