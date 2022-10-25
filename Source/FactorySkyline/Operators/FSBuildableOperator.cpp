// ILikeBanas


#include "FactorySkyline/Operators/FSBuildableOperator.h"

#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSBuilder.h"
#include "FactorySkyline/Operators/FSBuildingOperator.h"
#include "FactorySkyline/Operators/FSConveyorOperator.h"
#include "FactorySkyline/Operators/FSConveyorBeltOperator.h"
#include "FactorySkyline/Operators/FSConveyorLiftOperator.h"
#include "FactorySkyline/Operators/FSConveyorPoleOperator.h"
#include "FactorySkyline/Operators/FSConveyorPoleStackableOperator.h"
#include "FactorySkyline/Operators/FSFactoryOperator.h"
#include "FactorySkyline/Operators/FSFoundationOperator.h"
#include "FactorySkyline/Operators/FSGeneratorOperator.h"
#include "FactorySkyline/Operators/FSJumppadOperator.h"
#include "FactorySkyline/Operators/FSLadderOperator.h"
#include "FactorySkyline/Operators/FSManufacturerOperator.h"
#include "FactorySkyline/Operators/FSPipelineSupportOperator.h"
#include "FactorySkyline/Operators/FSPipelineSupportStkOperator.h"
#include "FactorySkyline/Operators/FSPipelineOperator.h"
#include "FactorySkyline/Operators/FSPowerPoleOperator.h"
#include "FactorySkyline/Operators/FSPumpOperator.h"
#include "FactorySkyline/Operators/FSRailroadSwitchOperator.h"
#include "FactorySkyline/Operators/FSRailroadTrackOperator.h"
#include "FactorySkyline/Operators/FSResourceExtractorOperator.h"
#include "FactorySkyline/Operators/FSConveyorAttachmentOperator.h"
#include "FactorySkyline/Operators/FSConveyorSmartSplitterOperator.h"
#include "FactorySkyline/Operators/FSStorageOperator.h"
#include "FactorySkyline/Operators/FSTowerOperator.h"
#include "FactorySkyline/Operators/FSTradingPostOperator.h"
#include "FactorySkyline/Operators/FSTrainPlatformOperator.h"
#include "FactorySkyline/Operators/FSTrainPlatformCargoOperator.h"
#include "FactorySkyline/Operators/FSTrainStationOperator.h"
#include "FactorySkyline/Operators/FSWalkwayOperator.h"
#include "FactorySkyline/Operators/FSWallOperator.h"
#include "FactorySkyline/Operators/FSWireOperator.h"

#include "FactorySkyline/Operators/FSBuildablePassThroughOperator.h"

#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableAttachmentMerger.h"
#include "Buildables/FGBuildableAttachmentSplitter.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Buildables/FGBuildableConveyorLift.h"
#include "Buildables/FGBuildableFactory.h"
#include "Buildables/FGBuildableFactoryBuilding.h"
#include "Buildables/FGBuildableFoundation.h"
#include "Buildables/FGBuildableJumppad.h"
#include "Buildables/FGBuildableLadder.h"
#include "Buildables/FGBuildableManufacturer.h"
#include "Buildables/FGBuildablePoleBase.h"
#include "Buildables/FGBuildablePole.h"
#include "Buildables/FGBuildableWire.h"
#include "Buildables/FGBuildableGenerator.h"
#include "Buildables/FGBuildablePipeline.h"
#include "Buildables/FGBuildablePipelinePump.h"
#include "Buildables/FGBuildablePipeHyper.h"
#include "Buildables/FGBuildablePowerPole.h"
#include "Buildables/FGBuildableRadarTower.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableRailroadSwitchControl.h"
#include "Buildables/FGBuildableRailroadTrack.h"
#include "Buildables/FGBuildableResourceExtractor.h"
#include "Buildables/FGBuildableSplitterSmart.h"
#include "Buildables/FGBuildableStorage.h"
#include "Buildables/FGBuildableTradingPost.h"
#include "Buildables/FGBuildableTrainPlatform.h"
#include "Buildables/FGBuildableTrainPlatformCargo.h"
#include "Buildables/FGBuildableWalkway.h"
#include "Buildables/FGBuildableWall.h"
#include "Buildables/FGConveyorPoleStackable.h"

#include "Hologram/FGHologram.h"
#include "Hologram/FGConveyorBeltHologram.h"
#include "Hologram/FGPipelineHologram.h"
#include "Resources/FGBuildingDescriptor.h"
#include "FGBuildablePipelineSupport.h"
#include "FGBuildableSubsystem.h"
#include "FGFactoryConnectionComponent.h"
#include "FGFactoryLegsComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGPowerConnectionComponent.h"
#include "FGRecipe.h"
#include "FGRecipeManager.h"

#include <string>
#include <exception>
#include <iostream>

#include<chrono>
#include<thread>
using namespace std;


AFGHologram* UFSBuildableOperator::CreateHologram()
{
	TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(Source->GetClass());
	if (!Recipe) return nullptr;
	AFGHologram* Hologram = AFGHologram::SpawnHologramFromRecipe(Recipe, Builder, FVector(0.0f, 0.0f, 0.0f), ((AFSkyline*)Skyline)->FSCtrl->GetPlayer());

	FHitResult Hit;
	Hit.Actor = nullptr;
	Hit.Time = 0.006946;
	Hit.Location = FVector(-11720.067f, 248538.719f, -10141.936f);
	Hit.ImpactPoint = FVector(-11720.066f, 248538.719f, -10141.936f);
	Hit.Normal = FVector(1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-11025.803f, 248538.188f, -10162.381f);
	Hit.TraceEnd = FVector(-110982.445f, 248615.406f, -12781.198f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	if (Hologram) Hologram->SetHologramLocationAndRotation(Hit);

	return Hologram;
}

void UFSBuildableOperator::UpdateHologramState(const FHitResult& Hit, AFGHologram* Hologram, bool& ShouldShow, bool& Valid)
{
	ShouldShow = Valid = false;
	if (!Hologram->IsValidHitResult(Hit)) return;
	
	if (Hologram->TrySnapToActor(Hit)) {
		ShouldShow = Valid = true;
		return;
	}
	
	Hologram->SetHologramLocationAndRotation(Hit);
	ShouldShow = Valid = true;
}

AFGHologram* UFSBuildableOperator::HologramCopy(FTransform& RelativeTransform)
{
	RelativeTransform = Source->GetTransform();
	return CreateHologram();
}

AFGBuildable* UFSBuildableOperator::CreateCopy(const FSTransformOperator& TransformOperator)
{
	//std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));
	FTransform Transform = TransformOperator.Transform(Source->GetTransform());
	//FTransform Transform = FTransform ();

	sourceClass = Source->GetClass();
	Buildable = nullptr;

	//TSubclassOf< UFGRecipe > ConveyorPoleRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Foundations/Recipe_Foundation_8x1_01.Recipe_Foundation_8x1_01_C"));
	//try {
		Buildable = BuildableSubsystem->BeginSpawnBuildable(sourceClass, Transform);
		//if (Buildable == nullptr) return nullptr;

	//}
	//catch (exception& e) {
		//cout << "Standard exception: " << e.what() << endl;
	//}

	//if (sourceClass->ClassGeneratedBy == nullptr) {
		//return nullptr;
	//}

	//TSubclassOf<UFGRecipe> Recipe = SplineHologramFactory->GetRecipeFromClass(sourceClass);
	//if (!Recipe) Recipe = Source->GetBuiltWithRecipe();
	//if (!Recipe) return nullptr;

	//Buildable->SetBuiltWithRecipe(Recipe);
	//TODO:
	//Buildable->SetBuildingID(Source->GetBuildingID());

	//std::string  str = TCHAR_TO_UTF8(*Buildable->GetName());
	//str.append("\n");
	//std::wstring temp = std::wstring(str.begin(), str.end());
	//LPCWSTR wideString = temp.c_str();
	//OutputDebugStringW(wideString);
	//FFactoryCustomizationData data = Source->GetCustomizationData_Implementation();
	/*
	FFactoryCustomizationData data = FFactoryCustomizationData();
	FFactoryCustomizationData data2 = Source->mCustomizationData;
	Buildable->mHologramClass = Source->mHologramClass;
	Buildable->mDisplayName = Source->mDisplayName;
	Buildable->mDescription = Source->mDescription;
	Buildable->MaxRenderDistance = Source->MaxRenderDistance;
	Buildable->mHighlightVector = Source->mHighlightVector;
	Buildable->mDecoratorClass = Source->mDecoratorClass;
	Buildable->mDefaultSwatchCustomizationOverride = Source->mDefaultSwatchCustomizationOverride;
	//Buildable->SetCustomizationData_Implementation(Source->mCustomizationData);
	//Buildable->SetCustomizationData_Native(Source->mCustomizationData);
	Buildable->SetCustomizationData_Implementation(data2);
	Buildable->mCustomizationData.ColorSlot = Source->mColorSlot;
	TSubclassOf< UFGFactoryCustomizationDescriptor_Swatch > SwatchDesc = TSubclassOf< UFGFactoryCustomizationDescriptor_Swatch >();
	data.SwatchDesc = SwatchDesc;
	Buildable->OnRep_CustomizationData();
	*/
	//TSubclassOf< UFGFactoryCustomizationDescriptor_Skin > skin1 = Buildable->GetActiveSkin();
	//TSubclassOf< UFGFactoryCustomizationDescriptor_Skin > skin2 = Buildable->GetActiveSkin();
	//auto skin1 = Source->GetActiveSkin_Native();
	//auto skin2 = Buildable->GetActiveSkin_Native();
	//skin2.SwatchDesc = skin1.SwatchDesc;
	//TSubclassOf< class UFGFactoryCustomizationDescriptor_Swatch > swatch = Source->swatch;
	//Buildable->ApplySkinData(skinImp);
	//Buildable->OnSkinCustomizationApplied(skinImp);
	//Buildable->OverrideColorData =
	// doesn't appear to save any color slot data in any scenario
	//Buildable->mCustomizationData.ColorSlot = Source->mColorSlot;

	//Source->GetCustomizationData();
	// crashes game
	//Buildable->SetCustomizationData(Source->GetCustomizationData());
	// crashes game
	//Buildable->ApplyCustomizationData(Source->GetCustomizationData());
	//Buildable->ApplyCustomizationData_Native(Source->GetCustomizationData());

	//Buildable->FlagReevaluateMaterialOnColored();
	//Source->OnRep_CustomizationData();
	//Buildable->OnRep_CustomizationData();
	//Buildable->OnSkinCustomizationApplied(Source->GetActiveSkin_Implementation());

	//TSubclassOf< UFGFactoryCustomizationDescriptor_Skin > skinImp = Source->UFGFactoryCustomizationDescriptor_Skin();
	//Buildable->ApplySkinData(skinImp);
	//Buildable->OnSkinCustomizationApplied(skinImp);

	/*
	AFSkyline* FSkyline = AFSkyline::Get(this);
	// hack to refresh static mesh - This is needed for power indicators and meshes to update properly.
	auto OutlineComp = UFGOutlineComponent::Get(FSkyline->FSCtrl->World);
	OutlineComp->ShowDismantlePendingMaterial(Buildable);
	OutlineComp->HideAllDismantlePendingMaterial();
	*/


	//Buildable->OnSkinCustomizationApplied(Source->TSubclassOf< class UFGFactoryCustomizationDescriptor_Skin >());

	//Buildable->mColorSlot = Source->mColorSlot;
	//uint8 color = Buildable->mColorSlot;
	//Buildable->SetCustomizationData_Implementation(Source->GetCustomizationData_Implementation());

	Buildable->FinishSpawning(Transform);

	UFGFactoryLegsComponent* Legs = Buildable->FindComponentByClass<UFGFactoryLegsComponent>();
	if (Legs) {
		TArray< FFeetOffset > feetOffset = Legs->TraceFeetOffsets(Transform, Buildable);
		Legs->SetFeetOffsets(feetOffset);
		Legs->RecreateLegs();
	}

	/*
	SML::Logging::info(TEXT("UFSBuildableOperator::CreateCopy"));
	SML::Logging::info(*Source->GetFullName());
	for (UActorComponent* Component : Source->GetComponents()) {
		SML::Logging::info(*Component->GetFullName());
		UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component);
		if (MeshComponent) {
			for (UMaterialInterface* MaterialInterface : MeshComponent->GetMaterials())
				SML::Logging::info(*MaterialInterface->GetFullName());
		}
	}*/
	//this->firstBuild = true;
	return Buildable;
}

void UFSBuildableOperator::UpdateInternelConnection(AFGBuildable* Buildable)
{
	TArray<UActorComponent*> SourceComponent = Source->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	TArray<UActorComponent*> TargetComponent = Buildable->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	for (UActorComponent* SourceConnection : SourceComponent) {
		UFGConnectionComponent* SFC = Cast<UFGConnectionComponent>(SourceConnection);
		for (UActorComponent* TargetConnection : TargetComponent) {
			UFGConnectionComponent* TFC = Cast<UFGConnectionComponent>(TargetConnection);
			if (SFC && TFC && SFC->GetName().Equals(TFC->GetName(), ESearchCase::Type::IgnoreCase)) {
				this->InternalConnectionMapping->Add(SFC, TFC);
			}
		}
	}
}

void UFSBuildableOperator::ApplyConnection(AFGBuildable* Buildable, UFGConnectionComponent* SFC, UFGConnectionComponent* TFC, const FSTransformOperator& TransformOperator, bool Force)
{
}

void UFSBuildableOperator::ApplyConnection(AFGBuildable* Buildable, const FSTransformOperator& TransformOperator, bool Force)
{
	TArray<UActorComponent*> SourceComponent = Source->GetComponentsByClass(UFGConnectionComponent::StaticClass());
	for (UActorComponent* SourceConnection : SourceComponent) if (!Cast<UFGPowerConnectionComponent>(SourceConnection)) {
		UFGConnectionComponent* SFC = Cast<UFGConnectionComponent>(SourceConnection);
		UFGConnectionComponent* TFC = ConnectionMapping<UFGConnectionComponent>(SFC);
		if (SFC && TFC) ApplyConnection(Buildable, SFC, TFC, TransformOperator, Force);
	}
}

void UFSBuildableOperator::ApplySettingsTo(AFGBuildable* Buildable)
{

}

FSBuildableType UFSBuildableOperator::GetType() const
{
	return FSBuildableType::Building;
}

void UFSBuildableOperator::GetSelectConnectList(AFGBuildable* Buildable, TArray<TWeakObjectPtr<AFGBuildable>>& List) const
{
}

UFSBuildableOperator* UFSBuildableOperator::SetUnknown()
{
	Unknown = true;
	return this;
}


void UFSOperatorFactory::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	this->World = FSkyline->World;
	this->Skyline = FSkyline;
	this->Builder = FSkyline->Builder;
	this->SplineHologramFactory = FSkyline->SplineHologramFactory;
	this->BuildableSubsystem = AFGBuildableSubsystem::Get(this);
	this->BuildableService = FSkyline->BuildableService;
}

UFSBuildableOperator* UFSOperatorFactory::AcquireOperator(AFGBuildable* Buildable)
{
	if (!Buildable) return nullptr;

	UFSBuildableOperator** Ptr = Map.Find(Buildable->GetClass());
	UFSBuildableOperator* Result;

	if (Ptr) {
		Result = *Ptr;
		Result->LoadBuildable(Buildable);
	}
	else {
		Result = CreateOperator(Buildable);
		Map.Add(Buildable->GetClass(), Result);
	}

	Result->BuildableMapping = BuildableMapping;
	Result->InternalConnectionMapping = InternalConnectionMapping;
	//Result->BucketIDMapping = BucketIDMapping;
	//Result->BucketMapping = BucketMapping;

	return Result;
}

UFSBuildableOperator* UFSOperatorFactory::CreateOperator(AFGBuildable* Buildable)
{
	UFSBuildableOperator* Result = CreateEmptyOperator(Buildable->GetClass());
	if (!Result) return nullptr;

	InitOperator(Result);

	Result->LoadBuildable(Buildable);

	return Result;
}

UFSBuildableOperator* UFSOperatorFactory::EnsureCache(UClass* Buildable)
{
	if (!Buildable) return nullptr;

	UFSBuildableOperator** Ptr = Map.Find(Buildable);
	if (Ptr) return *Ptr;

	UFSBuildableOperator* Result = CreateEmptyOperator(Buildable);
	if (!Result) return nullptr;

	InitOperator(Result);
	Map.Add(Buildable, Result);

	return Result;
}

void UFSOperatorFactory::InitOperator(UFSBuildableOperator* Operator)
{
	Operator->World = this->World;
	Operator->Skyline = this->Skyline;
	Operator->Builder = this->Builder;
	Operator->SplineHologramFactory = this->SplineHologramFactory;
	Operator->BuildableSubsystem = this->BuildableSubsystem;
	Operator->BuildableService = this->BuildableService;

	Operator->TypeCache = Operator->GetType();
}

UFSBuildableOperator* UFSOperatorFactory::CreateEmptyOperator(UClass* Buildable)
{
	if (Buildable->IsChildOf<AFGBuildableFactoryBuilding>()) {
		if (Buildable->IsChildOf<AFGBuildableFoundation>()) {
			return NewObject<UFSFoundationOperator>(this);
		}
		if (Buildable->IsChildOf<AFGBuildableWall>()) {
			return NewObject<UFSWallOperator>(this);
		}
		if (Buildable->IsChildOf<AFGBuildableWalkway>()) {
			return NewObject<UFSWalkwayOperator>(this);
		}
		
		return NewObject<UFSBuildingOperator>(this)->SetUnknown();

	}
	if (Buildable->IsChildOf<AFGBuildableConveyorBase>()) {
		if (Buildable->IsChildOf<AFGBuildableConveyorBelt>()) {
			if (CheckContains(Buildable, "Build_ConveyorBeltMk")) return NewObject<UFSConveyorBeltOperator>(this);
			return NewObject<UFSConveyorBeltOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableConveyorLift>()) {
			if (CheckContains(Buildable, "Build_ConveyorLiftMk")) return NewObject<UFSConveyorLiftOperator>(this);
			return NewObject<UFSConveyorLiftOperator>(this)->SetUnknown();
		}
		return NewObject<UFSConveyorOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildableFactory>()) {
		if (Buildable->IsChildOf<AFGBuildableManufacturer>()) {
			if (Check(Buildable, "Build_SmelterMk1_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_FoundryMk1_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_ConstructorMk1_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_AssemblerMk1_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_ManufacturerMk1_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_Packager_C")) return NewObject<UFSManufacturerOperator>(this);
			if (Check(Buildable, "Build_OilRefinery_C")) return NewObject<UFSManufacturerOperator>(this);
			return NewObject<UFSManufacturerOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableGenerator>()) {
			if (Check(Buildable, "Build_GeneratorBiomass_C")) return NewObject<UFSGeneratorOperator>(this);
			if (Check(Buildable, "Build_GeneratorCoal_C")) return NewObject<UFSGeneratorOperator>(this);
			if (Check(Buildable, "Build_GeneratorFuel_C")) return NewObject<UFSGeneratorOperator>(this);
			if (Check(Buildable, "Build_GeneratorGeoThermal_C")) return NewObject<UFSGeneratorOperator>(this);
			if (Check(Buildable, "Build_GeneratorNuclear_C")) return NewObject<UFSGeneratorOperator>(this);
			return NewObject<UFSGeneratorOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableConveyorAttachment>()) {
			if (Buildable->IsChildOf<AFGBuildableSplitterSmart>()) {
				if (Check(Buildable, "Build_ConveyorAttachmentSplitterSmart_C")) return NewObject<UFSConveyorSmartSplitterOperator>(this);
				if (Check(Buildable, "Build_ConveyorAttachmentSplitterProgrammable_C")) return NewObject<UFSConveyorSmartSplitterOperator>(this);
				return NewObject<UFSConveyorSmartSplitterOperator>(this)->SetUnknown();
			}
			if (Check(Buildable, "Build_ConveyorAttachmentMerger_C")) return NewObject<UFSConveyorAttachmentOperator>(this);
			if (Check(Buildable, "Build_ConveyorAttachmentSplitter_C")) return NewObject<UFSConveyorAttachmentOperator>(this);
			return NewObject<UFSConveyorAttachmentOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableResourceExtractor>()) {
			if (CheckContains(Buildable, "Build_MinerMk")) return NewObject<UFSResourceExtractorOperator>(this);
			if (Check(Buildable, "Build_OilPump_C")) return NewObject<UFSResourceExtractorOperator>(this);
			if (Check(Buildable, "Build_WaterPump_C")) return NewObject<UFSResourceExtractorOperator>(this);
			return NewObject<UFSResourceExtractorOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableStorage>()) {
			if (CheckContains(Buildable, "Build_StorageContainerMk")) return NewObject<UFSStorageOperator>(this);
			if (Check(Buildable, "Build_StoragePlayer_C")) return NewObject<UFSStorageOperator>(this);
			if (Check(Buildable, "Build_PipeStorageTank_C")) return NewObject<UFSStorageOperator>(this);
			if (Check(Buildable, "Build_IndustrialTank_C")) return NewObject<UFSStorageOperator>(this);
			return NewObject<UFSStorageOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildablePipelinePump>()) {
			if (Check(Buildable, "Build_PipelinePump_C")) return NewObject<UFSPumpOperator>(this);
			if (Check(Buildable, "Build_PipelinePumpMk2_C")) return NewObject<UFSPumpOperator>(this);
			if (Check(Buildable, "Build_Valve_C")) return NewObject<UFSPumpOperator>(this);
			return NewObject<UFSPumpOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableJumppad>()) {
			if (Check(Buildable, "Build_JumpPad_C")) return NewObject<UFSJumppadOperator>(this);
			if (Check(Buildable, "Build_JumpPadAdjustable_C")) return NewObject<UFSJumppadOperator>(this);
			if (Check(Buildable, "Build_JumpPadTilted_C")) return NewObject<UFSJumppadOperator>(this);
			return NewObject<UFSJumppadOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableRadarTower>()) {
			if (Check(Buildable, "Build_RadarTower_C")) return NewObject<UFSTowerOperator>(this);
			return NewObject<UFSTowerOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableTradingPost>()) {
			if (Check(Buildable, "Build_TradingPost_C")) return NewObject<UFSTradingPostOperator>(this);
			return NewObject<UFSTradingPostOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildableTrainPlatform>()) {
			if (Buildable->IsChildOf<AFGBuildableRailroadStation>()) {
				if (Check(Buildable, "Build_TrainStation_C")) return NewObject<UFSTrainStationOperator>(this);
				return NewObject<UFSTrainStationOperator>(this)->SetUnknown();
			}
			if (Buildable->IsChildOf<AFGBuildableTrainPlatformCargo>()) {
				if (Check(Buildable, "Build_TrainDockingStation_C")) return NewObject<UFSTrainPlatformCargoOperator>(this);
				if (Check(Buildable, "Build_TrainDockingStationLiquid_C")) return NewObject<UFSTrainPlatformCargoOperator>(this);
				return NewObject<UFSTrainPlatformCargoOperator>(this)->SetUnknown();
			}
			if (Check(Buildable, "Build_TrainPlatformEmpty_C")) return NewObject<UFSTrainPlatformOperator>(this);
			return NewObject<UFSTrainPlatformOperator>(this)->SetUnknown();
		}
		if (Check(Buildable, "Build_PipeHyperStart_C")) return NewObject<UFSFactoryOperator>(this);
		if (Check(Buildable, "Build_PipelineJunction_Cross_C")) return NewObject<UFSFactoryOperator>(this);
		if (Check(Buildable, "Build_ResourceSink_C")) return NewObject<UFSFactoryOperator>(this);
		if (Check(Buildable, "Build_ResourceSinkShop_C")) return NewObject<UFSTowerOperator>(this);
		if (Check(Buildable, "Build_LandingPad_C")) return NewObject<UFSTowerOperator>(this);
		if (Check(Buildable, "Build_LookoutTower_C")) return NewObject<UFSTowerOperator>(this);
		if (Check(Buildable, "Build_SpaceElevator_C")) return NewObject<UFSTowerOperator>(this);
		return NewObject<UFSFactoryOperator>(this)->SetUnknown();
	}
	if(Buildable->IsChildOf<AFGBuildablePoleBase>()) {
		if (Buildable->IsChildOf<AFGBuildablePole>()) {
			if (Buildable->IsChildOf<AFGConveyorPoleStackable>()) {
				if (Check(Buildable, "Build_ConveyorPoleStackable_C")) return NewObject<UFSConveyorPoleStackableOperator>(this);
				NewObject<UFSConveyorPoleStackableOperator>(this)->SetUnknown();
			}
			if (Check(Buildable, "Build_ConveyorPole_C")) return NewObject<UFSConveyorPoleOperator>(this);
			return NewObject<UFSConveyorPoleOperator>(this)->SetUnknown();
		}
		if (Buildable->IsChildOf<AFGBuildablePipelineSupport>()) {
			if (Check(Buildable, "Build_PipelineSupport_C")) return NewObject<UFSPipelineSupportOperator>(this);
			if (Check(Buildable, "Build_PipeSupportStackable_C")) return NewObject<UFSPipelineSupportStkOperator>(this);
			if (Check(Buildable, "Build_PipeHyperSupport_C")) return NewObject<UFSPipelineSupportOperator>(this);
			if (Check(Buildable, "Build_HyperPoleStackable_C")) return NewObject<UFSPipelineSupportStkOperator>(this);
			return NewObject<UFSPipelineSupportOperator>(this)->SetUnknown();
		}
		NewObject<UFSConveyorPoleStackableOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildableLadder>()) {
		if (Check(Buildable, "Build_Ladder_C")) return NewObject<UFSLadderOperator>(this);
		return NewObject<UFSLadderOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildableWire>()) {
		if (Check(Buildable, "Build_PowerLine_C")) return NewObject<UFSWireOperator>(this);
		return NewObject<UFSWireOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildablePowerPole>()) {
		if (CheckContains(Buildable, "Build_PowerPoleMk")) return NewObject<UFSPowerPoleOperator>(this);
		if (Check(Buildable, "Build_PowerPoleWall_C")) return NewObject<UFSPowerPoleOperator>(this);
		if (CheckContains(Buildable, "Build_PowerPoleWall_Mk")) return NewObject<UFSPowerPoleOperator>(this);
		if (Check(Buildable, "Build_PowerPoleWallDouble_C")) return NewObject<UFSPowerPoleOperator>(this);
		if (CheckContains(Buildable, "Build_PowerPoleWallDouble_Mk")) return NewObject<UFSPowerPoleOperator>(this);
		return NewObject<UFSPowerPoleOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildablePipeBase>()) {
		if (Check(Buildable, "Build_Pipeline_C")) return NewObject<UFSPipelineOperator>(this);
		if (Check(Buildable, "Build_PipelineMK2_C")) return NewObject<UFSPipelineOperator>(this);
		if (Check(Buildable, "Build_PipeHyper_C")) return NewObject<UFSPipelineOperator>(this);
		return NewObject<UFSPipelineOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildableRailroadTrack>()) {
		if (Check(Buildable, "Build_RailroadTrack_C")) return NewObject<UFSRailroadTrackOperator>(this);
		if (Check(Buildable, "Build_RailroadTrackIntegrated_C")) return NewObject<UFSRailroadTrackOperator>(this);
		return NewObject<UFSRailroadTrackOperator>(this)->SetUnknown();
	}
	if (Buildable->IsChildOf<AFGBuildableRailroadSwitchControl>()) {
		//if (Check(Buildable, "Build_RailroadSwitchControl_C")) return NewObject<UFSRailroadSwitchOperator>(this);
		return NewObject<UFSRailroadSwitchOperator>(this)->SetUnknown();
	}

	if (Check(Buildable, "Build_FoundationPassthrough_Lift_C")) {
		return NewObject <UFSBuildablePassThroughOperator>(this);
	}

	if (Check(Buildable, "Build_Mam_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_WorkBench_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_Workshop_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_ConveyorPoleWall_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_HyperTubeWallHole_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_HyperTubeWallSupport_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_PipelineSupportWall_C")) return NewObject<UFSBuildableOperator>(this);
	if (Check(Buildable, "Build_PipelineSupportWallHole_C")) return NewObject<UFSBuildableOperator>(this);

	return NewObject<UFSBuildableOperator>(this)->SetUnknown();
}

FSTransformOperator::FSTransformOperator(const FTransform& Source, const FTransform& Target)
{
	this->SourceTransform = Source;
	this->TargetTransform = Target;
}

FTransform FSTransformOperator::Transform(const FTransform& Transform) const
{
	FVector RelativeVector = SourceTransform.InverseTransformPositionNoScale(Transform.GetLocation());
	FQuat RelativeRotation = SourceTransform.InverseTransformRotation(Transform.GetRotation());

	FTransform NewTransform(TargetTransform.TransformRotation(RelativeRotation),
		TargetTransform.TransformPositionNoScale(RelativeVector), Transform.GetScale3D());

	return NewTransform;
}


FVector FSTransformOperator::Transform(const FVector& Vector) const
{
	FVector RelativeVector = SourceTransform.InverseTransformPositionNoScale(Vector);
	return TargetTransform.TransformPositionNoScale(RelativeVector);
}

FQuat FSTransformOperator::Transform(const FQuat& Quat) const
{
	FQuat RelativeQuat = SourceTransform.InverseTransformRotation(Quat);
	return TargetTransform.TransformRotation(RelativeQuat);
}

FTransform FSTransformOperator::InverseTransform(const FTransform& Transform) const
{
	FVector RelativeVector = TargetTransform.InverseTransformPositionNoScale(Transform.GetLocation());
	FQuat RelativeRotation = TargetTransform.InverseTransformRotation(Transform.GetRotation());

	FTransform NewTransform(SourceTransform.TransformRotation(RelativeRotation),
		SourceTransform.TransformPositionNoScale(RelativeVector), Transform.GetScale3D());

	return NewTransform;
}

void UFSSplineHologramFactory::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);

	//this->FGBuildGun = FSkyline->FGBuildGun;
	this->Skyline = FSkyline;
	this->Builder = FSkyline->Builder;
	this->BuildableSubsystem = AFGBuildableSubsystem::Get(this);
}

void UFSSplineHologramFactory::Load()
{
	ConveyorPole = nullptr;
	PipelineSupport = nullptr;
	PipelineHyperSupport = nullptr;

	RecipesMapping.Empty();

	TArray< TSubclassOf< UFGRecipe > > out_recipes;
	AFGRecipeManager::Get(this)->GetAllAvailableRecipes(out_recipes);
	out_recipes.Add(LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Recipe_RailroadTrackIntegrated.Recipe_RailroadTrackIntegrated_C")));

	for (TSubclassOf< UFGRecipe > RecipeClass : out_recipes) {
		TArray< FItemAmount > Items = UFGRecipe::GetProducts(RecipeClass);
		if (Items.Num() == 1 && Items[0].ItemClass) {
			TSubclassOf< UFGBuildingDescriptor > BuildingDescriptorClass = (TSubclassOf< UFGBuildingDescriptor >)(UFGRecipe::GetDescriptorForRecipe(RecipeClass));
			if (BuildingDescriptorClass) {
				UClass* BuildableClass =  UFGBuildingDescriptor::GetBuildableClass(BuildingDescriptorClass);
				if (BuildableClass) {
					RecipesMapping.Add(BuildableClass, RecipeClass);
					//SML::Logging::info(*RecipeClass->GetFullName());
					//SML::Logging::info(*UFGBuildingDescriptor::GetBuildableClass(BuildingDescriptorClass)->GetFullName());
				}
			}
		}
	}
	/*
	FGBuildGun->GetAvailableRecipes(CacheRecipes);
	for (TSubclassOf< class UFGRecipe > Recipes : CacheRecipes) {
		UClass* BuildableClass = AFGBuildable::GetBuildableClassFromRecipe(Recipes);
		if (BuildableClass) RecipesMapping.Add(BuildableClass, Recipes);
	}*/

}

void UFSSplineHologramFactory::Unload()
{
	if (ConveyorPole) ConveyorPole->TurnOffAndDestroy();
	if (PipelineSupport) PipelineSupport->TurnOffAndDestroy();
	if (PipelineHyperSupport) PipelineHyperSupport->TurnOffAndDestroy();
	
}

TSubclassOf<UFGRecipe> UFSSplineHologramFactory::GetRecipeFromClass(TSubclassOf<AFGBuildable> BuildableClass)
{
	const TSubclassOf<UFGRecipe>* Result = RecipesMapping.Find(BuildableClass);
	return Result ? *Result : nullptr;
}
/*
AFGConveyorBeltHologram* UFSSplineHologramFactory::CreateBeltHologram(AFGBuildableConveyorBelt* ConveyorBelt, FTransform& RelativeTransform)
{
	TSubclassOf< UFGRecipe > ConveyorPoleRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Recipe_ConveyorPole.Recipe_ConveyorPole_C"));

	if (!ConveyorPole) {
		FTransform Transform(FRotator(0.0f, 0.0f, 0.0f), FVector(-53821.339844f, 244785.484375f, -13746.879639f));
		ConveyorPole = Cast<AFGBuildablePole>(BuildableSubsystem->BeginSpawnBuildable(AFGBuildable::GetBuildableClassFromRecipe(ConveyorPoleRecipe), Transform));
		ConveyorPole->SetBuiltWithRecipe(ConveyorPoleRecipe);
		ConveyorPole->FinishSpawning(Transform);
		ConveyorPole->SetActorHiddenInGame(true);
	}

	FVector Vevtor0 = ConveyorBelt->GetConnection0()->GetConnectorLocation();
	FRotator Rotator0 = ConveyorBelt->GetConnection0()->GetConnectorNormal().ToOrientationRotator();
	FVector Vevtor1 = ConveyorBelt->GetConnection1()->GetConnectorLocation();
	FRotator Rotator1 = ConveyorBelt->GetConnection1()->GetConnectorNormal().ToOrientationRotator();

	FTransform ConveyorBeltStart(Rotator0, Vevtor0);
	FTransform ConveyorBeltEnd(Rotator1, Vevtor1);

	RelativeTransform = ConveyorBeltStart;

	FHitResult Hit;
	Hit.Actor = ConveyorPole;
	Hit.Time = 0.005515f;
	Hit.Location = FVector(-53839.797f, 244792.078f, -13691.415f);
	Hit.ImpactPoint = FVector(-53839.797f, 244792.078f, -13691.415f);
	Hit.Normal = FVector(-1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(-1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-54390.883f, 244772.328f, -13682.122f);
	Hit.TraceEnd = FVector(45530.727f, 248354.750f, -15367.185f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	TSubclassOf<UFGRecipe> HologramRecipe = this->GetRecipeFromClass(ConveyorBelt->GetClass());
	if (!HologramRecipe) return nullptr;

	AFGConveyorBeltHologram* ConveyorBeltHologram = Cast<AFGConveyorBeltHologram>(AFGHologram::SpawnHologramFromRecipe(HologramRecipe, Builder, FVector(), ((AFSkyline*)Skyline)->FSCtrl->GetPlayer()));
	ConveyorBeltHologram->SetHologramLocationAndRotation(Hit);
	ConveyorBeltHologram->DoMultiStepPlacement(false);

	FSTransformOperator FSTransform(ConveyorBeltEnd, ConveyorBeltHologram->GetTransform());
	ConveyorBeltHologram->SetActorTransform(FSTransform.Transform(ConveyorBeltStart));

	ConveyorBeltHologram->SetHologramLocationAndRotation(Hit);

	return ConveyorBeltHologram;
}*/
#pragma optimize ("", off)
AFGConveyorLiftHologram* UFSSplineHologramFactory::CreateLiftHologram(AFGBuildableConveyorLift* ConveyorLift, FTransform& RelativeTransform)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	TSubclassOf< UFGRecipe > ConveyorPoleRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Recipe_ConveyorPole.Recipe_ConveyorPole_C"));
	if (!ConveyorPole) {
		FTransform Transform(FRotator(0.0f, 0.0f, 0.0f), FVector(-53821.339844f, 244785.484375f, -13746.879639f));
		ConveyorPole = Cast<AFGBuildablePole>(BuildableSubsystem->BeginSpawnBuildable(AFGBuildable::GetBuildableClassFromRecipe(ConveyorPoleRecipe), Transform));
		ConveyorPole->SetBuiltWithRecipe(ConveyorPoleRecipe);
		ConveyorPole->FinishSpawning(Transform);
		ConveyorPole->SetActorHiddenInGame(true);
	}
	/*
	FVector Vevtor0 = ConveyorLift->GetConnection0()->GetConnectorLocation();
	FRotator Rotator0 = ConveyorLift->GetConnection0()->GetConnectorNormal().ToOrientationRotator();
	FVector Vevtor1 = ConveyorLift->GetConnection1()->GetConnectorLocation();
	FRotator Rotator1 = ConveyorLift->GetConnection1()->GetConnectorNormal().ToOrientationRotator();
	*/
	FVector Vevtor0 = FSkyline->AdaptiveUtil->GetConveyorConnection(ConveyorLift, 0)->GetConnectorLocation();
	FRotator Rotator0 = FSkyline->AdaptiveUtil->GetConveyorConnection(ConveyorLift, 0)->GetConnectorNormal().ToOrientationRotator();
	FVector Vevtor1 = FSkyline->AdaptiveUtil->GetConveyorConnection(ConveyorLift, 1)->GetConnectorLocation();
	FRotator Rotator1 = FSkyline->AdaptiveUtil->GetConveyorConnection(ConveyorLift, 1)->GetConnectorNormal().ToOrientationRotator();

	FTransform ConveyorLiftStart(Rotator0, Vevtor0);
	FTransform ConveyorLiftEnd(Rotator1, Vevtor1);

	RelativeTransform = ConveyorLiftStart;
	FHitResult Hit;
	Hit.Actor = ConveyorPole;
	Hit.Time = 0.005515f;
	Hit.Location = FVector(-53839.797f, 244792.078f, -13691.415f);
	Hit.ImpactPoint = FVector(-53839.797f, 244792.078f, -13691.415f);
	Hit.Normal = FVector(-1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(-1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-54390.883f, 244772.328f, -13682.122f);
	Hit.TraceEnd = FVector(45530.727f, 248354.750f, -15367.185f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	TSubclassOf<UFGRecipe> HologramRecipe = this->GetRecipeFromClass(ConveyorLift->GetClass());
	if (!HologramRecipe) return nullptr;
	Log("%s", (*UFGRecipe::GetRecipeName(HologramRecipe).ToString()));

	// developer note, passing FVector() is unsafe use FVector(0.0f, 0.0f, 0.0f) instead
	AFGHologram* SpawnedLiftHologram = AFGHologram::SpawnHologramFromRecipe(HologramRecipe, Builder, FVector(0.0f, 0.0f, 0.0f), ((AFSkyline*)Skyline)->FSCtrl->GetPlayer());
	AFGConveyorLiftHologram* ConveyorLiftHologram = Cast<AFGConveyorLiftHologram>(SpawnedLiftHologram);
	//auto ConveyorLiftHologram = SpawnedLiftHologram;
	if (ConveyorLiftHologram != nullptr) {
		// probably a check that won't fix this?
	}
	ConveyorLiftHologram->SetHologramLocationAndRotation(Hit);
	ConveyorLiftHologram->DoMultiStepPlacement(false);

	FSTransformOperator FSTransform(ConveyorLiftEnd, ConveyorLiftHologram->GetTransform());
	ConveyorLiftHologram->SetActorTransform(FSTransform.Transform(ConveyorLiftStart));

	float y = (Rotator0 - Rotator1).Yaw;
	if (y > 0) y -= 360.0f;
	while (y < -45.0f) {
		ConveyorLiftHologram->Scroll(1);
		y += 90.0f;
	}

	ConveyorLiftHologram->SetHologramLocationAndRotation(Hit);

	return ConveyorLiftHologram;
}
#pragma optimize ("", on)

AFGPipelineHologram* UFSSplineHologramFactory::CreatePipelineHologram(AFGBuildablePipeline* Pipeline, FTransform& RelativeTransform)
{
	TSubclassOf< UFGRecipe > PipelineSupportRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Recipe_PipeSupport.Recipe_PipeSupport_C"));

	FTransform SupportTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(-11766.815430f, 248540.609375f, -10324.533142f));
	if (!PipelineSupport) {
		PipelineSupport = Cast<AFGBuildablePipelineSupport>(BuildableSubsystem->BeginSpawnBuildable(AFGBuildable::GetBuildableClassFromRecipe(PipelineSupportRecipe), SupportTransform));
		PipelineSupport->SetBuiltWithRecipe(PipelineSupportRecipe);
		PipelineSupport->SetSupportLength(100.0f);
		PipelineSupport->FinishSpawning(SupportTransform);
		PipelineSupport->GetRootComponent()->SetMobility(EComponentMobility::Type::Movable);
		PipelineSupport->SetActorHiddenInGame(true);
	}
	else {
		PipelineSupport->SetActorTransform(SupportTransform);
	}

	FVector Vevtor0 = Pipeline->GetConnection0()->GetConnectorLocation();
	FRotator Rotator0 = (-Pipeline->GetConnection0()->GetConnectorNormal()).ToOrientationRotator();
	FVector Vevtor1 = Pipeline->GetConnection1()->GetConnectorLocation();
	FRotator Rotator1 = (-Pipeline->GetConnection1()->GetConnectorNormal()).ToOrientationRotator();

	FTransform PipelineStart(Rotator0, Vevtor0);
	FTransform PipelineEnd(Rotator1, Vevtor1);

	RelativeTransform = PipelineStart;

	FHitResult Hit;
	Hit.Actor = PipelineSupport;
	Hit.Time = 0.006946;
	Hit.Location = FVector(-11720.067f, 248538.719f, -10141.936f);
	Hit.ImpactPoint = FVector(-11720.066f, 248538.719f, -10141.936f);
	Hit.Normal = FVector(1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-11025.803f, 248538.188f, -10162.381f);
	Hit.TraceEnd = FVector(-110982.445f, 248615.406f, -12781.198f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	TSubclassOf<UFGRecipe> HologramRecipe = this->GetRecipeFromClass(Pipeline->GetClass());
	if (!HologramRecipe) return nullptr;

	AFGPipelineHologram* PipelineHologram = Cast<AFGPipelineHologram>(AFGHologram::SpawnHologramFromRecipe(HologramRecipe, Builder, FVector(0.0f, 0.0f, 0.0f), ((AFSkyline*)Skyline)->FSCtrl->GetPlayer()));
	PipelineHologram->SetHologramLocationAndRotation(Hit);
	PipelineHologram->DoMultiStepPlacement(false);

	FSTransformOperator FSTransform(PipelineStart, PipelineSupport->GetTransform());
	FTransform Transform = FSTransform.Transform(PipelineEnd);
	PipelineSupport->SetActorTransform(Transform);

	FSTransformOperator FSTransformHit(PipelineHologram->GetTransform(), Transform);
	Hit.Location = FSTransformHit.Transform(Hit.Location);
	Hit.ImpactPoint = FSTransformHit.Transform(Hit.ImpactPoint);
	Hit.Normal = FSTransformHit.Transform(Hit.Normal.ToOrientationQuat()).Vector();
	Hit.ImpactNormal = FSTransformHit.Transform(Hit.ImpactNormal.ToOrientationQuat()).Vector();
	Hit.TraceStart = FSTransformHit.Transform(Hit.TraceStart);
	Hit.TraceEnd = FSTransformHit.Transform(Hit.TraceEnd);

	PipelineHologram->SetHologramLocationAndRotation(Hit);

	return PipelineHologram;
}


AFGPipelineHologram* UFSSplineHologramFactory::CreatePipelineHologram(AFGBuildablePipeHyper* Pipeline, FTransform& RelativeTransform)
{
	TSubclassOf< UFGRecipe > PipelineHyperSupportRecipe = LoadClass<UFGRecipe>(this, TEXT("/Game/FactoryGame/Recipes/Buildings/Recipe_PipeHyperSupport.Recipe_PipeHyperSupport_C"));

	FTransform SupportTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(-1766.815430f, 248540.609375f, -10324.533142f));
	if (!PipelineHyperSupport) {
		PipelineHyperSupport = Cast<AFGBuildablePipelineSupport>(BuildableSubsystem->BeginSpawnBuildable(AFGBuildable::GetBuildableClassFromRecipe(PipelineHyperSupportRecipe), SupportTransform));
		PipelineHyperSupport->SetBuiltWithRecipe(PipelineHyperSupportRecipe);
		PipelineHyperSupport->SetSupportLength(100.0f);
		PipelineHyperSupport->FinishSpawning(SupportTransform);
		PipelineHyperSupport->GetRootComponent()->SetMobility(EComponentMobility::Type::Movable);
		PipelineHyperSupport->SetActorHiddenInGame(true);
	}
	else {
		PipelineHyperSupport->SetActorTransform(SupportTransform);
	}

	FVector Vevtor0 = Pipeline->GetConnection0()->GetConnectorLocation();
	FRotator Rotator0 = (-Pipeline->GetConnection0()->GetConnectorNormal()).ToOrientationRotator();
	FVector Vevtor1 = Pipeline->GetConnection1()->GetConnectorLocation();
	FRotator Rotator1 = (-Pipeline->GetConnection1()->GetConnectorNormal()).ToOrientationRotator();

	FTransform PipelineStart(Rotator0, Vevtor0);
	FTransform PipelineEnd(Rotator1, Vevtor1);

	RelativeTransform = PipelineStart;

	FHitResult Hit;
	Hit.Actor = PipelineHyperSupport;
	Hit.Time = 0.006946;
	Hit.Location = FVector(-1720.067f, 248538.719f, -10141.936f);
	Hit.ImpactPoint = FVector(-1720.066f, 248538.719f, -10141.936f);
	Hit.Normal = FVector(1.0f, 0.0f, 0.0f);
	Hit.ImpactNormal = FVector(1.0f, 0.0f, 0.0f);
	Hit.TraceStart = FVector(-1025.803f, 248538.188f, -10162.381f);
	Hit.TraceEnd = FVector(-10982.445f, 248615.406f, -12781.198f);
	Hit.PenetrationDepth = 0.0f;
	Hit.Item = -1;
	Hit.FaceIndex = -1;

	TSubclassOf<UFGRecipe> HologramRecipe = this->GetRecipeFromClass(Pipeline->GetClass());
	if (!HologramRecipe) return nullptr;

	AFGPipelineHologram* PipelineHologram = Cast<AFGPipelineHologram>(AFGHologram::SpawnHologramFromRecipe(HologramRecipe, Builder, FVector(0.0f, 0.0f, 0.0f), ((AFSkyline*)Skyline)->FSCtrl->GetPlayer()));
	PipelineHologram->SetHologramLocationAndRotation(Hit);
	PipelineHologram->DoMultiStepPlacement(false);

	FSTransformOperator FSTransform(PipelineStart, PipelineHyperSupport->GetTransform());
	FTransform Transform = FSTransform.Transform(PipelineEnd);
	PipelineHyperSupport->SetActorTransform(Transform);

	FSTransformOperator FSTransformHit(PipelineHologram->GetTransform(), Transform);
	Hit.Location = FSTransformHit.Transform(Hit.Location);
	Hit.ImpactPoint = FSTransformHit.Transform(Hit.ImpactPoint);
	Hit.Normal = FSTransformHit.Transform(Hit.Normal.ToOrientationQuat()).Vector();
	Hit.ImpactNormal = FSTransformHit.Transform(Hit.ImpactNormal.ToOrientationQuat()).Vector();
	Hit.TraceStart = FSTransformHit.Transform(Hit.TraceStart);
	Hit.TraceEnd = FSTransformHit.Transform(Hit.TraceEnd);


	PipelineHologram->SetHologramLocationAndRotation(Hit);

	return PipelineHologram;
}

