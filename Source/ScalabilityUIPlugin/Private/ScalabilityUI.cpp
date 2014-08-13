// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ScalabilityUIPrivatePCH.h"
#include "Slate.h"
#include "ScalabilityUIPanel.h"

class FScalabilityUIPlugin : public IScalabilityUIPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FScalabilityUIPlugin, ScalabilityUIPlugin)

void FScalabilityUIPlugin::StartupModule()
{
	if (!IsRunningDedicatedServer() && IsRunningGame())
	{
		//Create a Scalability UI Window, and display it modally (blocks progress until it's closed)
		TSharedRef<SScalabilityUIPanel> ScalabilityUIWindow = SNew(SScalabilityUIPanel);

		//Show it as modal (blocking until it's closed)
		TSharedPtr<SWindow> ParentWindow;
		FSlateApplication::Get().AddModalWindow(ScalabilityUIWindow, ParentWindow);

		if (ScalabilityUIWindow->GetReturnCode() == 0)
		{
			FPlatformMisc::RequestExit(true);
		}
	}
}

void FScalabilityUIPlugin::ShutdownModule()
{
}



