


#include "AutoCollectFoliage_BPFL.h"
#include "FGFoliageRemovalSubsystem.h"
#include "FGFoliageTypes.h"



	FFoliageInstanceStableId UAutoCollectFoliage_BPFL::GetInstanceHashOne(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, int32 InstanceId)
	{
		return target->GetStableInstanceId(Component, InstanceId);
	}


	FFoliageInstanceStableId UAutoCollectFoliage_BPFL::GetInstanceHashTwo(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, const FTransform& Transform)
	{
		return target->GetStableInstanceId(Component, Transform);
};
