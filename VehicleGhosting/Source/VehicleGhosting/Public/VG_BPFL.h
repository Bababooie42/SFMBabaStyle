

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WheeledVehicles/FGTargetPointLinkedList.h"
#include "FGVehicleSubsystem.h"
#include "VG_BPFL.generated.h"

/**
 * 
 */

UCLASS()
class VEHICLEGHOSTING_API UVG_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "VG_BPFL")
	static float GetCAD(AFGDrivingTargetList* target);
	
	UFUNCTION(BlueprintCallable, Category = "VG_BPFL")
	static const float SetCAD(AFGDrivingTargetList* target, float newCAD = 0);

	//UFUNCTION(BlueprintCallable, Category = "VG_BPFL")

	//static void ResetBeingTheChosenWheeledVehicle(AFGVehicleSubsystem* target, const class AFGWheeledVehicleInfo* oldChosenVehicle, bool moveToHardDeadlocks);

};
