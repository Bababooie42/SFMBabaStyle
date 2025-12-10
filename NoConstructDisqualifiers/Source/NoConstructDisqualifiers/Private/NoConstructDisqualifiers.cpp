// Copyright Epic Games, Inc. All Rights Reserved.

#include "NoConstructDisqualifiers.h"
#include "Hologram/FGHologram.h"
#include "Hologram/FGBuildableHologram.h"
#include "Hologram/FGResourceExtractorHologram.h"
#include "Hologram/FGWaterPumpHologram.h"
#include "Hologram/FGRailroadTrackHologram.h"
#include "Hologram/FGPipelineHologram.h"
#include "Hologram/FGPipeAttachmentHologram.h"
#include "Hologram/FGConveyorBeltHologram.h"
#include "Hologram/FGWireHologram.h"
#include "FGConstructDisqualifier.h"
#include <SessionSettings/SessionSettingsManager.h>
#include "FGInputLibrary.h"
#include "Patching/NativeHookManager.h"
#include "ModConfig_NoConstructDisqualifiersStruct.h"

#define LOCTEXT_NAMESPACE "FNoConstructDisqualifiersModule"

void FNoConstructDisqualifiersModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterHook();
}

void FNoConstructDisqualifiersModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

float savedMin = 100000;
float savedMaxLength = 100000;
float savedMaxAngle = 350;



void FNoConstructDisqualifiersModule::RegisterHook()
{
	if (!WITH_EDITOR)
	{
		AFGHologram* Holog = GetMutableDefault<AFGHologram>();
		AFGBuildableHologram* Bolog = GetMutableDefault<AFGBuildableHologram>();
		AFGResourceExtractorHologram* Rolog = GetMutableDefault<AFGResourceExtractorHologram>();
		AFGWaterPumpHologram* Wolog = GetMutableDefault<AFGWaterPumpHologram>();
		AFGPipeAttachmentHologram* Jolog = GetMutableDefault<AFGPipeAttachmentHologram>();
		AFGWireHologram* Polog = GetMutableDefault<AFGWireHologram>();
		AFGRailroadTrackHologram* tolog = GetMutableDefault< AFGRailroadTrackHologram>();


		SUBSCRIBE_METHOD(AFGHologram::CanConstruct, [](auto& scope, const AFGHologram* self) {

			if (self->GetConstructionInstigator())
			{
				auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
				USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
				auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
				auto ignoreCost = SessionSettings->GetBoolOptionValue("NCD.IgnoreCost");
				FKey AllowConstructKey;
				TArray<FKey> ModifierKeys;
				UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

				if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
				{
					if (!self->mConstructDisqualifiers.Contains(UFGCDUnaffordable::StaticClass()) || ignoreCost)
					{
						scope.Override(true);
					}
				}
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGRailroadTrackHologram::CanTakeNextBuildStep, tolog, [](auto& scope, const AFGRailroadTrackHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto ignoreCost = SessionSettings->GetBoolOptionValue("NCD.IgnoreCost");
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				auto holo = Cast<AFGHologram>(self);
				if (!holo->mConstructDisqualifiers.Contains(UFGCDUnaffordable::StaticClass()) || ignoreCost)
				{
					scope.Override(true);
				}
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGResourceExtractorHologram::ConfigureActor, Rolog, [](auto& scope, const AFGResourceExtractorHologram* self, class AFGBuildable* inBuildable) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				scope.Cancel();
				self->AFGBuildableHologram::ConfigureActor(inBuildable);

			}
			});

		/*

		SUBSCRIBE_METHOD_VIRTUAL(AFGBuildableHologram::CheckValidFloor, Bolog, [](auto& scope, AFGBuildableHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGResourceExtractorHologram::CheckValidPlacement, Rolog, [](auto& scope, AFGResourceExtractorHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGWaterPumpHologram::CheckValidPlacement, Wolog, [](auto& scope, AFGWaterPumpHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGHologram::CheckClearance, Holog, [](auto& scope, AFGHologram* self) {
			scope(self);
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
			{
				self->mConstructDisqualifiers.Empty();
			}
			});

		SUBSCRIBE_METHOD(AFGRailroadTrackHologram::ValidateGrade, [](auto& scope, AFGRailroadTrackHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
			{
				scope.Override(true);
			}
			});

		*/

		SUBSCRIBE_METHOD(AFGRailroadTrackHologram::ValidateCurvature, [](auto& scope, AFGRailroadTrackHologram* self) {

			if (self->mMinLength != 0)
			{
				savedMin = self->mMinLength;
			}
			if (self->mMaxLength != 100000)
			{
				savedMaxLength = self->mMaxLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				if (self->mMinLength != 0)
				{
					self->mMinLength = 0;
				}
				if (self->mMaxLength < 100000)
				{
					self->mMaxLength = 100000;
				}
				scope.Override(true);
			}
			else
			{
				if (self->mMinLength != savedMin)
				{
					self->mMinLength = savedMin;
				}
				if (self->mMaxLength != savedMaxLength)
				{
					self->mMaxLength = savedMaxLength;
				}
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGPipeAttachmentHologram::CheckValidPlacement, Jolog, [](auto& scope, AFGPipeAttachmentHologram* self) {

			if (self->mMaxValidTurnAngle != 350)
			{
				savedMaxAngle = self->mMaxValidTurnAngle;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				if (self->mMaxValidTurnAngle != 350)
				{
					self->mMaxValidTurnAngle = 350;
				}
				scope(self);
			}
			else
			{
				if (self->mMaxValidTurnAngle != savedMaxAngle)
				{
					self->mMaxValidTurnAngle = savedMaxAngle;
				}
			}

			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateMinLength, [](auto& scope, AFGPipelineHologram* self) {

			if (self->mMaxSplineLength != 100000)
			{
				savedMaxLength = self->mMaxSplineLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				if (self->mMaxSplineLength < 100000)
				{
					self->mMaxSplineLength = 100000;
				}
				scope.Override(true);
			}
			else
			{
				if (self->mMaxSplineLength != savedMaxLength)
				{
					self->mMaxSplineLength = savedMaxLength;
				}
			}

			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateCurvatureAndReturnFaultyPosition, [](auto& scope, AFGPipelineHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				scope.Override(0.f);
			}
			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateFluidRequirements, [](auto& scope, AFGPipelineHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				scope.Override(true);
			}
			});

		SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateMinLength, [](auto& scope, AFGConveyorBeltHologram* self) {

			if (self->mMaxSplineLength != 100000)
			{
				savedMaxLength = self->mMaxSplineLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			USessionSettingsManager* SessionSettings = self->GetWorld()->GetSubsystem<USessionSettingsManager>();
			auto alwaysOn = SessionSettings->GetBoolOptionValue("NCD.AlwaysOn");
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey) || alwaysOn)
			{
				if (self->mMaxSplineLength < 100000)
				{
					self->mMaxSplineLength = 100000;
				}
				scope.Override(true);
			}
			else
			{
				if (self->mMaxSplineLength != savedMaxLength)
				{
					self->mMaxSplineLength = savedMaxLength;
				}
			}

			});
		/*
	SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateIncline, [](auto& scope, AFGConveyorBeltHologram* self) {

		auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
		auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
		FKey AllowConstructKey;
		TArray<FKey> ModifierKeys;
		UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

		if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
		{
			scope.Override(true);
		}
		});

	SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateCurvature, [](auto& scope, AFGConveyorBeltHologram* self) {

		auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
		auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
		FKey AllowConstructKey;
		TArray<FKey> ModifierKeys;
		UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

		if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
		{
			scope.Override(true);
		}
		});

	SUBSCRIBE_METHOD_VIRTUAL(AFGWireHologram::CheckValidPlacement, Polog, [](auto& scope, AFGWireHologram* self) {

		auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
		auto config = FModConfig_NoConstructDisqualifiersStruct::GetActiveConfig(self->GetWorld());
		FKey AllowConstructKey;
		TArray<FKey> ModifierKeys;
		UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

		if (contr->IsInputKeyDown(AllowConstructKey) || config.AlwaysOn)
		{
			scope.Cancel();
		}
		});

	*/

	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNoConstructDisqualifiersModule, NoConstructDisqualifiers)