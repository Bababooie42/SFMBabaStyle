

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FGFoliageRemovalSubsystem.h"
#include "FGFoliageTypes.h"
#include "FolInst_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class FOLIAGEINSTANCEBPFL_API UFolInst_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "FolInst_BPFL")
	static FFoliageInstanceStableId GetInstanceHashByInstanceID(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, int32 InstanceId);

	UFUNCTION(BlueprintCallable, Category = "FolInst_BPFL")
	static FFoliageInstanceStableId GetInstanceHashByInstanceTransform(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, const FTransform& Transform);

};
