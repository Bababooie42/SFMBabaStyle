// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Patching/NativeHookManager.h"
#include "FGRailroadSubsystem.h"
//#include "FGVehicleSubsystem.h"
#include "Modules/ModuleManager.h"

class FVehicleGhostingModule : public IModuleInterface
{
	/** IModuleInterface implementation */
	void StartupModule() override;
	
	void ShutdownModule() override;
	
	void registerHooks();
	
};
