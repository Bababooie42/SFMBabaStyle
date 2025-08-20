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
#include "FGInputLibrary.h"
#include "Patching/NativeHookManager.h"

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

float savedMin;
float savedMax;




void FNoConstructDisqualifiersModule::RegisterHook()
{
	if (!WITH_EDITOR)
	{
		AFGHologram* Holog = GetMutableDefault<AFGHologram>();
		AFGBuildableHologram* Bolog = GetMutableDefault<AFGBuildableHologram>();
		AFGResourceExtractorHologram* Rolog = GetMutableDefault<AFGResourceExtractorHologram>();
		AFGWaterPumpHologram* Wolog = GetMutableDefault<AFGWaterPumpHologram>();
		AFGPipeAttachmentHologram* Jolog = GetMutableDefault<AFGPipeAttachmentHologram>();

		SUBSCRIBE_METHOD_VIRTUAL(AFGBuildableHologram::CheckValidFloor, Bolog, [](auto& scope, AFGBuildableHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGResourceExtractorHologram::CheckValidPlacement, Rolog, [](auto& scope, AFGResourceExtractorHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGResourceExtractorHologram::ConfigureActor, Rolog, [](auto& scope, const AFGResourceExtractorHologram* self, class AFGBuildable* inBuildable) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Cancel();
				self->AFGBuildableHologram::ConfigureActor(inBuildable);
				
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGWaterPumpHologram::CheckValidPlacement, Wolog, [](auto& scope, AFGWaterPumpHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Cancel();
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGHologram::CheckClearance, Holog, [](auto& scope, AFGHologram* self) {
			scope(self);
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				self->mConstructDisqualifiers.Empty();
			}
			});

		SUBSCRIBE_METHOD(AFGRailroadTrackHologram::ValidateGrade, [](auto& scope, AFGRailroadTrackHologram* self) {
			
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Override(true);
			}
			});
		
		
		
		SUBSCRIBE_METHOD(AFGRailroadTrackHologram::ValidateCurvature, [](auto& scope, AFGRailroadTrackHologram* self) {
			
			if (self->mMinLength != 0)
			{
				savedMin = self->mMinLength;
			}
			if (self->mMaxLength < 100000)
			{
				savedMax = self->mMaxLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
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
				if (self->mMaxLength != savedMax)
				{
					self->mMaxLength = savedMax;
				}
			}
			});

		SUBSCRIBE_METHOD_VIRTUAL(AFGPipeAttachmentHologram::CheckValidPlacement, Jolog, [](auto& scope, AFGPipeAttachmentHologram* self) {

			if (self->mMaxValidTurnAngle != 350)
			{
				savedMax = self->mMaxValidTurnAngle;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				if (self->mMaxValidTurnAngle != 350)
				{
					self->mMaxValidTurnAngle = 350;
				}
				scope(self);
			}
			else
			{
				if (self->mMaxValidTurnAngle != savedMax)
				{
					self->mMaxValidTurnAngle = savedMax;
				}
			}

			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateMinLength, [](auto& scope, AFGPipelineHologram* self) {
			
			if (self->mMaxSplineLength < 100000)
			{
				savedMax = self->mMaxSplineLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				if (self->mMaxSplineLength < 100000)
				{
					self->mMaxSplineLength = 100000;
				}
				scope.Override(true);
			}
			else
			{
				if (self->mMaxSplineLength != savedMax)
				{
					self->mMaxSplineLength = savedMax;
				}
			}

			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateCurvatureAndReturnFaultyPosition, [](auto& scope, AFGPipelineHologram* self) {
			
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Override(0.f);
			}
			});

		SUBSCRIBE_METHOD(AFGPipelineHologram::ValidateFluidRequirements, [](auto& scope, AFGPipelineHologram* self) {
			
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Override(true);
			}
			});

		SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateMinLength, [](auto& scope, AFGConveyorBeltHologram* self) {

			if (self->mMaxSplineLength < 100000)
			{
				savedMax = self->mMaxSplineLength;
			}
			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				if (self->mMaxSplineLength < 100000)
				{
					self->mMaxSplineLength = 100000;
				}
				scope.Override(true);
			}
			else
			{
				if (self->mMaxSplineLength != savedMax)
				{
					self->mMaxSplineLength = savedMax;
				}
			}

			});

		SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateIncline, [](auto& scope, AFGConveyorBeltHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Override(true);
			}
			});

		SUBSCRIBE_METHOD(AFGConveyorBeltHologram::ValidateCurvature, [](auto& scope, AFGConveyorBeltHologram* self) {

			auto contr = Cast<APlayerController>(self->GetConstructionInstigator()->GetController());
			FKey AllowConstructKey;
			TArray<FKey> ModifierKeys;
			UFGInputLibrary::GetCurrentMappingForAction(contr, "BuildGunBuild_AllowConstruct", AllowConstructKey, ModifierKeys);

			if (contr->IsInputKeyDown(AllowConstructKey))
			{
				scope.Override(true);
			}
			});

	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNoConstructDisqualifiersModule, NoConstructDisqualifiers)