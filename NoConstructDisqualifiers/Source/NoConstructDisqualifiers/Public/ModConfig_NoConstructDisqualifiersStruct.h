#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "ModConfig_NoConstructDisqualifiersStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/NoConstructDisqualifiers/ModConfig_NoConstructDisqualifiers' */
USTRUCT(BlueprintType)
struct FModConfig_NoConstructDisqualifiersStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    bool AlwaysOn{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FModConfig_NoConstructDisqualifiersStruct GetActiveConfig(UObject* WorldContext) {
        FModConfig_NoConstructDisqualifiersStruct ConfigStruct{};
        FConfigId ConfigId{"NoConstructDisqualifiers", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FModConfig_NoConstructDisqualifiersStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

