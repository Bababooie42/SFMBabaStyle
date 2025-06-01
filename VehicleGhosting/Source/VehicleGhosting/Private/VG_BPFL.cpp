


#include "VG_BPFL.h"
#include "WheeledVehicles/FGTargetPointLinkedList.h"
#include "FGVehicleSubsystem.h"




float UVG_BPFL::GetCAD(AFGDrivingTargetList* target)
{
	return target->mCollisionAvoidanceDistance;
}

const float UVG_BPFL::SetCAD(AFGDrivingTargetList* target, float newCAD)
{
	return target->mCollisionAvoidanceDistance = newCAD ;
}

//void UVG_BPFL::ResetBeingTheChosenWheeledVehicle(AFGVehicleSubsystem* target, const class AFGWheeledVehicleInfo* oldChosenVehicle, bool moveToHardDeadlocks)
//{
//	target->ResetBeingTheChosenWheeledVehicle(oldChosenVehicle, moveToHardDeadlocks);
//}

