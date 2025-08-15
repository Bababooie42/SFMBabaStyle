

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FGFoliageRemovalSubsystem.h"
#include "FGFoliageTypes.h"
#include "AutoCollectFoliage_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class AUTOCOLLECTFOLIAGE_API UAutoCollectFoliage_BPFL : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "ACF_BPFL")
	static FFoliageInstanceStableId GetInstanceHashOne(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, int32 InstanceId);

	UFUNCTION(BlueprintCallable, Category = "ACF_BPFL")
	static FFoliageInstanceStableId GetInstanceHashTwo(AFGFoliageRemovalSubsystem* target, UHierarchicalInstancedStaticMeshComponent* Component, const FTransform& Transform);

	
};
