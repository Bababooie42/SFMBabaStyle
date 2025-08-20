


#include "FolInst_BPFL.h"
#include "FGFoliageRemovalSubsystem.h"
#include "FGFoliageTypes.h"

FFoliageInstanceStableId UFolInst_BPFL::GetInstanceHashByInstanceID(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, int32 InstanceId)
{
	return target->GetStableInstanceId(Component, InstanceId);
}


FFoliageInstanceStableId UFolInst_BPFL::GetInstanceHashByInstanceTransform(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, const FTransform& Transform)
{
	return target->GetStableInstanceId(Component, Transform);
}