// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Patching/NativeHookManager.h"
#include "Modules/ModuleManager.h"


class FNoInvalidFloorPlacementModule : public IModuleInterface
{
	void StartupModule() override;
	void ShutdownModule() override;
	void RegisterHook();
};
