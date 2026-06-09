// Copyright Epic Games, Inc. All Rights Reserved.




#include "VehicleGhosting.h"
#include "Patching/NativeHookManager.h"
//#include "FGVehicleSubsystem.h"
//#include "FGWheeledVehicleInfo.h"
//#include "FGSplinePathMovementComponent.h"
//#include "FGTargetPointLinkedList.h"
#include "FGRailroadSubsystem.h"
#include <SessionSettings/SessionSettingsManager.h>
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FVehicleGhostingModule"

void FVehicleGhostingModule::StartupModule()
{
	registerHooks();
}



void FVehicleGhostingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



void FVehicleGhostingModule::registerHooks()
{
	if (!WITH_EDITOR)
	{

		/*SUBSCRIBE_METHOD (AFGVehicleSubsystem::IsVehicleBlockedBySource, [](auto& scope2, AFGVehicleSubsystem* self2, const AFGWheeledVehicleInfo* sourceVehicle2, const AFGWheeledVehicleInfo* blockedVehicle2, TSet< const AFGWheeledVehicleInfo* >& visitedVehicles2, TSet< AFGWheeledVehicleInfo* >& deadlockedVehicles2) {

			TArray< class AFGWheeledVehicleInfo* > wvi;
			wvi = self2->mWheeledVehicles;
			for (AFGWheeledVehicleInfo* wv : wvi)
			{
				wv->mSimulationMovement->ResetIsBlocked();
				wv->mSimulationMovement->ResetDeadlockedVehicles();
			}

			});*/
		SUBSCRIBE_METHOD(AFGRailroadSubsystem::SweepRailroadPositions, [](auto& scope, AFGRailroadSubsystem* self, FVector2D capsuleSize2, FRailroadTrackPosition startPos2, float startOrientation2, FRailroadTrackPosition endPos2, FVector2D otherCapsuleSize2, FRailroadTrackPosition otherPos2) {

			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto affectTrains = SessionSettings->GetBoolOptionValue("VG.AffectTrains");

			if (affectTrains)
			{
			float time = 1;
			scope.Override(time);
			}

			});

		
	}
}






#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVehicleGhostingModule, VehicleGhostingModule)
