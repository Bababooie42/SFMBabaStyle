// Copyright Epic Games, Inc. All Rights Reserved.



#include "NoInvalidFloorPlacement.h"
#include "FGConstructDisqualifier.h"
#include "Hologram/FGBuildableHologram.h"
#include "Hologram/FGHologram.h"
#include "Hologram/FGFactoryHologram.h"
#include "Patching/NativeHookManager.h"
#include "UObject/Class.h"
#include "FGFactoryLegsComponent.h"
#include <SessionSettings/SessionSettingsManager.h>
#include <Logging/StructuredLog.h>


#define LOCTEXT_NAMESPACE "FNoInvalidFloorPlacementModule"

float origLegLength;
bool DoneSet = false;
FVector pos;

void FNoInvalidFloorPlacementModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterHook();
}

void FNoInvalidFloorPlacementModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}


void FNoInvalidFloorPlacementModule::RegisterHook()
{
	

	if (!WITH_EDITOR)
	{
		AFGHologram* Holog = GetMutableDefault<AFGHologram>();
		AFGBuildableHologram* Bolog = GetMutableDefault<AFGBuildableHologram>();

		SUBSCRIBE_METHOD_VIRTUAL(AFGBuildableHologram::CheckValidFloor, Bolog, [](auto& scope, AFGBuildableHologram* self) {
		
			scope.Cancel();
			});


		SUBSCRIBE_METHOD_VIRTUAL(AFGHologram::CheckClearance, Holog, [](auto& scope, AFGHologram* self) {

			AFGBuildableHologram* bolo = Cast<AFGBuildableHologram>(self);
			
				if (bolo != nullptr)
				{
					if (bolo->mLegs != nullptr)
					{
						if (self->mConstructDisqualifiers.Contains(UFGCDInvalidFloor::StaticClass()))
						{
							self->mConstructDisqualifiers.Remove(UFGCDInvalidFloor::StaticClass());
							UE_LOG(LogNativeHookManager, Display, TEXT("Removed construct disqualifier?"));
						}
					}
				}
				else
				{
					UE_LOG(LogNativeHookManager, Display, TEXT("Cast failed!"));
				}
			});

		
		SUBSCRIBE_METHOD(UFGFactoryLegsComponent::CreateLegInstances, [](auto& scope, UFGFactoryLegsComponent* self, TArray<FInstanceData>& out_legInstanceData) {

			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto RemoveLegs = SessionSettings->GetBoolOptionValue("NIFP.CaptainDan");

			if (RemoveLegs)
			{
				scope.Cancel();
			}
			});
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNoInvalidFloorPlacementModule, NoInvalidFloorPlacement)