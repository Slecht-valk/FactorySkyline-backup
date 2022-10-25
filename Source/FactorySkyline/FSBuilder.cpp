// ILikeBanas


#include "FactorySkyline/FSBuilder.h"
#include "FactorySkyline/FSSelection.h"
#include "FactorySkyline/FSkyline.h"
#include "FactorySkyline/FSDesign.h"
#include "FactorySkyline/FSMenuItem.h"
#include "FactorySkyline/FSController.h"
#include "FactorySkyline/UI/FSkylineUI.h"
#include "FactorySkyline/UI/FSResourceIcon.h"
#include "FactorySkyline/Operators/FSBuildableOperator.h"
#include "FactorySkyline/Operators/FSConveyorLiftOperator.h"
#include "Equipment/FGBuildGun.h"
#include "FGBuildableSubsystem.h"
#include "FGFluidIntegrantInterface.h"
#include "FGRailroadSubsystem.h"
#include "FGPlayerController.h"
#include "FGPipeConnectionComponent.h"
#include "FGPipeSubsystem.h"
#include "FGRailroadTrackConnectionComponent.h"
#include "FGTrainStationIdentifier.h"
#include "FGGameState.h"
#include "Buildables/FGBuildable.h"
#include "Buildables/FGBuildableManufacturer.h"
#include "Buildables/FGBuildablePipeline.h"
#include "Buildables/FGBuildableConveyorBelt.h"
#include "Buildables/FGBuildableRailroadTrack.h"
#include "Buildables/FGBuildableRailroadStation.h"
#include "Buildables/FGBuildableWire.h"
#include "Hologram/FGHologram.h"
#include "Hologram/FGBuildableHologram.h"
#include "Hologram/FGFactoryHologram.h"
#include "Hologram/FGFoundationHologram.h"

void AFSBuilder::Init()
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return;

	this->World = FSkyline->World;
	this->FGController = FSkyline->FGController;

	this->OperatorFactory = FSkyline->OperatorFactory;
	this->SplineHologramFactory = FSkyline->SplineHologramFactory;
	this->BuildableService = FSkyline->BuildableService;
	this->FSCtrl = FSkyline->FSCtrl;
	this->SkylineUI = FSkyline->SkylineUI;
	
	this->SyncBuild = NewObject<UFSSyncBuild>(this);

	this->IsBuilding = false;

	this->Cost.Init(this->SplineHologramFactory);
	this->Inventory = &FSkyline->FSCtrl->Inventory;
}

bool AFSBuilder::CheckAnchor(UFSDesign* DesignParam)
{
	AnchorConnection = AnchorInput = AnchorOutput = nullptr;

	if (Cast<AFGBuildableConveyorBase>(DesignParam->Anchor)) {
		AFSkyline* FSkyline = AFSkyline::Get(this);
		AFGBuildableConveyorBase* Conveyor = Cast<AFGBuildableConveyorBase>(DesignParam->Anchor);
		//UFGFactoryConnectionComponent* Connection = Conveyor->GetConnection0();
		UFGFactoryConnectionComponent* Connection = FSkyline->AdaptiveUtil->GetConveyorConnection(Conveyor, 0);
		if (!Connection->IsConnected() || !DesignParam->IsElementSelected(Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor()))) AnchorInput = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorInput = Connection;
		}

		//Connection = Conveyor->GetConnection1();
		Connection = FSkyline->AdaptiveUtil->GetConveyorConnection(Conveyor, 1);
		if (!Connection->IsConnected() || !DesignParam->IsElementSelected(Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor()))) AnchorOutput = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorOutput = Connection;
		}

		return AnchorConnection || AnchorInput || AnchorOutput;
	}
	
	if (Cast<AFGBuildablePipeBase>(DesignParam->Anchor)) {
		AFGBuildablePipeBase* Pipe = Cast<AFGBuildablePipeBase>(DesignParam->Anchor);
		UFGPipeConnectionComponentBase* Connection = Pipe->GetConnection1();

		if (!Connection->IsConnected()) AnchorConnection = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorConnection = Connection;
		}

		Connection = Pipe->GetConnection0();
		if (!Connection->IsConnected()) AnchorConnection = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorConnection = Connection;
		}

		return AnchorConnection != nullptr;
	}

	if (Cast<AFGBuildableRailroadTrack>(DesignParam->Anchor)) {
		AFGBuildableRailroadTrack* Track = Cast<AFGBuildableRailroadTrack>(DesignParam->Anchor);
		UFGRailroadTrackConnectionComponent* Connection = Track->GetConnection(1);
		
		if (!Connection->IsConnected()) AnchorConnection = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorConnection = Connection;
		}

		Connection = Track->GetConnection(0);
		if (!Connection->IsConnected()) AnchorConnection = Connection;
		else {
			AFGBuildable* Buildable = Cast<AFGBuildable>(Connection->GetConnection()->GetAttachmentRootActor());
			if (!Buildable || !DesignParam->IsElementSelected(Buildable)) AnchorConnection = Connection;
		}

		return AnchorConnection != nullptr;
	}
	return true;
}

void AFSBuilder::Load(UFSDesign* DesignParam, bool FullPreview)
{
	AFSkyline* FSkyline = AFSkyline::Get(this);
	FSkyline->SkylineUI->CompletionWidget->SetVisibility(ESlateVisibility::Collapsed);

	SplineHologramFactory->Load();

	this->Design = DesignParam;
	this->Design->RecheckNullptr();

	AFGBuildable* AnchorBuildable = this->Design->Anchor.Get();
	this->AnchorOperator = OperatorFactory->CreateOperator(AnchorBuildable);

	this->Hologram = this->AnchorOperator->CreateHologram();
	if (this->Hologram == nullptr) return;
	//this->Hologram->SetActorHiddenInGame(false);
	//SML::Logging::info(*this->Hologram->GetFullName());

	FVector AnchorLocation = AnchorBuildable->GetTransform().GetLocation();
	if (AnchorOutput) AnchorLocation = AnchorOutput->GetComponentLocation();
	if (AnchorInput) AnchorLocation = AnchorInput->GetComponentLocation();
	if (AnchorConnection) AnchorLocation = AnchorConnection->GetComponentLocation();
	
	for (TWeakObjectPtr<AFGBuildable> BuildablePtr : DesignParam->BuildableSet) if (BuildablePtr.Get()) {
		AFGBuildable* Buildable = BuildablePtr.Get();

		float Dist = (Buildable->GetTransform().GetLocation() - AnchorLocation).Size();
		if (Buildable == AnchorBuildable || Dist < 10000.0f || FullPreview) {
			UFSBuildableOperator* BuildableOperator = OperatorFactory->AcquireOperator(Buildable);

			FTransform RelativeTransformVar;
			AFGHologram* HologramVar = BuildableOperator->HologramCopy(RelativeTransformVar);

			if (HologramVar) {
				this->HologramList.Add(HologramVar);
				this->RelativeTransform.Add(RelativeTransformVar);
			}
		}

	}
	
	Cost.Empty();
	AFGGameState* GameState = Cast<AFGGameState>(World->GetGameState());
	if (!GameState || !(GameState->GetCheatNoCost() == 1)) {
		Cost.AddResource(DesignParam);
	}

	LastShow = LastValid = false;

	SplineHologramFactory->Unload();

}

void AFSBuilder::Unload()
{
	for (AFGHologram* HologramVar : HologramList) {
		HologramVar->SetActorHiddenInGame(true);
		HologramVar->SetDisabled(true);
		HologramVar->Destroy();
	}
	HologramList.Empty();
	RelativeTransform.Empty();

	if (this->Hologram) {
		this->Hologram->SetActorHiddenInGame(true);
		this->Hologram->SetDisabled(true);
		this->Hologram->Destroy();
		this->Hologram = nullptr;
	}
	this->AnchorOperator = nullptr;
}

bool AFSBuilder::Test(const FHitResult& Hit)
{
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*Hit.GetActor()->GetFullName()));
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*Hit.GetActor()->GetTransform().ToString()));
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*this->Hologram->GetTransform().ToString()));
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*Hit.Location.ToString()));
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*Hit.TraceStart.ToString()));
	UE_LOG(LogSkyline, Verbose, TEXT("%s"), (*(FString::Printf(TEXT("bBlockingHit:%s\nbStartPenetrating:%s\nTime:%f\nLocation:%s\nImpactPoint:%s\nNormal:%s\nImpactNormal:%s\nTraceStart:%s\nTraceEnd:%s\nPenetrationDepth:%f\nItem:%d\nFaceIndex:%d"),
		Hit.bBlockingHit == true ? TEXT("True") : TEXT("False"),
		Hit.bStartPenetrating == true ? TEXT("True") : TEXT("False"),
		Hit.Time,
		*Hit.Location.ToString(),
		*Hit.ImpactPoint.ToString(),
		*Hit.Normal.ToString(),
		*Hit.ImpactNormal.ToString(),
		*Hit.TraceStart.ToString(),
		*Hit.TraceEnd.ToString(),
		Hit.PenetrationDepth,
		Hit.Item,
		Hit.FaceIndex))));
	
	//Hologram->DoMultiStepPlacement(false);
	/*
	AFGBuildableConveyorBelt* Belt = Cast<AFGBuildableConveyorBelt>(this->Design->ReferenceItem.Get());
	SML::Logging::info(*Belt->GetFullName());
	SML::Logging::info(*Belt->GetTransform().ToString());
	for (FSplinePointData Data : Belt->GetSplineData()) {
		SML::Logging::info(*Data.ToString());
	}

	SML::Logging::info(*Belt->GetConnection0()->GetFullName());
	SML::Logging::info(*Belt->GetConnection0()->GetConnectorLocation(false).ToString());
	SML::Logging::info(*Belt->GetConnection0()->GetConnectorNormal().ToString());
	SML::Logging::info(*Belt->GetConnection1()->GetFullName());
	SML::Logging::info(*Belt->GetConnection1()->GetConnectorLocation(false).ToString());
	SML::Logging::info(*Belt->GetConnection1()->GetConnectorNormal().ToString());
	*/

	//AFSkyline::Get(this)->ConveyorHologramFactory->CreateBeltHologram(Cast<AFGBuildableConveyorBelt>(this->Design->ReferenceItem.Get()));

	return false;
}

bool AFSBuilder::Build(FSRepeat* Repeat)
{
	if (!(this->LastShow && this->LastValid)) return false;
	if (!CheckCost()) return false;

	if (this->IsBuilding) {
		UE_LOG(LogSkyline, Verbose, TEXT("Warnning: FSBuilder call build while building"));
		return false;
	}
	if (!this->Design->Anchor.Get()) return false;

	if (this->Hologram == nullptr) return false;

	AFSkyline* FSkyline = AFSkyline::Get(this);
	if (!FSkyline) return false;

	SyncBuild->Load();

	UFSDesignMenu* CurrentDesignMenu = Cast<UFSDesignMenu>(FSkyline->SkylineUI->FindLastDesignItem()->CurrentExpandedItem);
	if (!CurrentDesignMenu) return false;

	for (int i = 0; i < this->HologramList.Num(); i++)
		this->HologramList[i]->SetActorHiddenInGame(true);
	LastShow = false;

	this->IsBuilding = true;
	FSkyline->SkylineUI->CompletionWidget->SetVisibility(ESlateVisibility::Collapsed);

	SyncBuild->Anchor = this->Design->Anchor;
	SyncBuild->BuildableSet = this->Design->BuildableSet;
	SyncBuild->CurrentDesignMenu = CurrentDesignMenu;
	SyncBuild->NewDesignMenu = UFSDesignMenu::CopyFrom(CurrentDesignMenu->ParentItem, CurrentDesignMenu, true);
	SyncBuild->NewDesign = SyncBuild->NewDesignMenu->Design;

	bool ValidTarget;
	this->Target = Repeat ? Repeat->Next() : GetFixedTargetTransform(this->Hologram, ValidTarget);
	this->Source = Repeat ? Repeat->Source : GetFixedSourceTransform();
	SyncBuild->FSTransform = FSTransformOperator(this->Source, this->Target);

	OperatorFactory->BuildableMapping = &SyncBuild->BuildableMapping;
	OperatorFactory->InternalConnectionMapping = &SyncBuild->InternalConnectionMapping;
	SyncBuild->OperatorFactory = this->OperatorFactory;

	SyncBuild->PreWork();
	//this->Consume();
	//SyncBuild->DoWork(1e6f);
	//SyncBuild->Unload();


	return true;
}

bool AFSBuilder::CheckReady(float TimeLimit)
{
	//SML::Logging::info(SyncBuild->GetCurrent(), TEXT(" "), SyncBuild->GetTotal());
	if (SyncBuild->DoWork(TimeLimit)) {
		this->IsBuilding = false;
		SyncBuild->Unload();
		return true;
	}
	return false;
}

void AFSBuilder::Update(const FHitResult& Hit)
{
	if (this->Hologram == nullptr) return;

	bool ShouldShow = false;
	bool Valid = false;
	bool ValidTarget = false;
	this->AnchorOperator->UpdateHologramState(Hit, this->Hologram, ShouldShow, Valid);

	//SML::Logging::info(*this->Hologram->GetActorLocation().ToString());

	if (IsBuilding) ShouldShow = false;

	if (ShouldShow) {
		this->Target = GetFixedTargetTransform(this->Hologram, ValidTarget);
		if (!ValidTarget) ShouldShow = false;
	}

	if (ShouldShow) {
		if (!CheckCost()) Valid = false;
	}

	if (LastShow != ShouldShow || ShouldShow) {

		FSTransformOperator FSTransform;
		if (ShouldShow) FSTransform = FSTransformOperator(this->Source = GetFixedSourceTransform(), this->Target);

		for (int i = 0; i < this->HologramList.Num(); i++) {
			if (LastShow != ShouldShow)	this->HologramList[i]->SetActorHiddenInGame(!ShouldShow);
			if (ShouldShow) this->HologramList[i]->SetActorTransform(FSTransform.Transform(this->RelativeTransform[i]));
			this->HologramList[i]->SetPlacementMaterialState(EHologramMaterialState::HMS_OK);
		}

		LastShow = ShouldShow;
		LastValid = Valid;
	}
}

void AFSBuilder::Update(FSRepeat* Repeat)
{
	if (!Repeat) return;
	FSTransformOperator FSTransform = FSTransformOperator(Repeat->Source, Repeat->Next());
	if (!IsBuilding) {
		CheckCost();
	}

	for (int i = 0; i < this->HologramList.Num(); i++) {
		this->HologramList[i]->SetActorHiddenInGame(false);
		this->HologramList[i]->SetPlacementMaterialState(EHologramMaterialState::HMS_OK);
		this->HologramList[i]->SetActorTransform(FSTransform.Transform(this->RelativeTransform[i]));
	}
}

FTransform AFSBuilder::GetFixedTargetTransform(AFGHologram* HologramParam, bool& Valid)
{
	FTransform Transform = HologramParam->GetTransform();
	Valid = true;

	if (Cast<AFGSplineHologram>(this->Hologram)) {
		AFGSplineHologram* SplineHologram = Cast<AFGSplineHologram>(this->Hologram);
		//SML::Logging::info(*SplineHologram->GetTransform().GetRotation().Rotator().ToString());
		
		AFGConveyorBeltHologram* BeltHologram = Cast<AFGConveyorBeltHologram>(this->Hologram);
		if (BeltHologram) {
			FSplinePointData Data;
			SplineHologram->GetLastSplineData(Data);
			Transform.SetRotation(Data.ArriveTangent.ToOrientationQuat());

			AnchorConnection = AnchorInput ? AnchorInput : AnchorOutput;
			UFGFactoryConnectionComponent* Connection = nullptr;
			if (AnchorInput && AnchorOutput) {
				Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Transform.GetLocation(), 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_INPUT);
				if (Connection) {
					if (Connection->GetDirection() == EFactoryConnectionDirection::FCD_SNAP_ONLY) AnchorConnection = AnchorInput;
					else AnchorConnection = AnchorOutput;
				}
				else AnchorConnection = AnchorInput;
			}
			else {
				if (AnchorInput) Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Transform.GetLocation(), 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_INPUT);
				if (AnchorOutput) Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Transform.GetLocation(), 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_OUTPUT);
				if (Connection) {
					Valid = (Connection->GetDirection() == EFactoryConnectionDirection::FCD_SNAP_ONLY);
				}
			}
		}

		UFSConveyorLiftOperator* LiftOperator = Cast<UFSConveyorLiftOperator>(AnchorOperator);
		if (LiftOperator && Cast<AFGConveyorLiftHologram>(this->Hologram)) {
			AnchorConnection = AnchorInput ? AnchorInput : AnchorOutput;
			UFGFactoryConnectionComponent* Connection = nullptr;
			if (LiftOperator->HitConnection) {
				FVector Location = LiftOperator->HitConnection->GetComponentTransform().GetLocation();
				if (AnchorInput && AnchorOutput) {
					if (LiftOperator->HitConnection->GetDirection() == EFactoryConnectionDirection::FCD_INPUT) AnchorConnection = AnchorOutput;
					else if (LiftOperator->HitConnection->GetDirection() == EFactoryConnectionDirection::FCD_OUTPUT ) AnchorConnection = AnchorInput;
					else {
						Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Location, 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_INPUT);
						if (Connection) {
							if (Connection->GetDirection() == EFactoryConnectionDirection::FCD_SNAP_ONLY) AnchorConnection = AnchorInput;
							else AnchorConnection = AnchorOutput;
						}
						else AnchorConnection = AnchorInput;
					}
				}
				else {
					if (AnchorInput) {
						if (LiftOperator->HitConnection->GetDirection() == EFactoryConnectionDirection::FCD_INPUT) Connection = LiftOperator->HitConnection;
						else Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Location, 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_INPUT);
					}
					if (AnchorOutput) {
						if (LiftOperator->HitConnection->GetDirection() == EFactoryConnectionDirection::FCD_OUTPUT) Connection = LiftOperator->HitConnection;
						else Connection = UFGFactoryConnectionComponent::FindOverlappingConnections(World, Location, 50.0f, EFactoryConnectionConnector::FCC_CONVEYOR, EFactoryConnectionDirection::FCD_OUTPUT);
					}
					if (Connection) {
						Valid = (Connection->GetDirection() == EFactoryConnectionDirection::FCD_SNAP_ONLY);
					}
				}
			}
		}

		if (Cast<AFGPipelineHologram>(this->Hologram)) {
			FSplinePointData Data;
			SplineHologram->GetLastSplineData(Data);
			Transform.SetRotation(Data.ArriveTangent.ToOrientationQuat());
		}

		if (Cast<AFGRailroadTrackHologram>(this->Hologram)) {
			FSplinePointData Data;
			SplineHologram->GetLastSplineData(Data);
			Transform.SetRotation(Data.ArriveTangent.ToOrientationQuat());
		}
	}

	return Transform;
}

FTransform AFSBuilder::GetFixedSourceTransform()
{
	FTransform Transform = this->Design->Anchor->GetTransform();
	if (Cast<AFGBuildableConveyorBase>(this->Design->Anchor) || Cast<AFGBuildablePipeBase>(this->Design->Anchor) || Cast<AFGBuildableRailroadTrack>(this->Design->Anchor)) {
		Transform = AnchorConnection->GetComponentTransform();
		if (Cast<AFGBuildableConveyorLift>(this->Design->Anchor)) return Transform;
		return FTransform((-Transform.GetRotation().Vector()).ToOrientationQuat(), Transform.GetLocation());
	}
	return Transform;
}

bool AFSBuilder::CheckCost()
{
	UFSkylineUI* SkylineUIVar = (UFSkylineUI*)this->SkylineUI;
	FSInventory Left = *Inventory;
	/*
	TMap<TSubclassOf<UFGItemDescriptor>, int> Minus;
	Left.AddResource(&Cost, -1);
	if (Left.Valid(Minus)) {
		SkylineUIVar->ItemBox->SetVisibility(ESlateVisibility::Collapsed);
		return true;
	}

	AFSController* FSCtrlVar = (AFSController*)this->FSCtrl;
	if (FSCtrlVar->GetPlayer()) {
		Left.AddResource(FSCtrlVar->GetPlayer()->GetInventory());
		if (Left.Valid(Minus)) {
			SkylineUIVar->ItemBox->SetVisibility(ESlateVisibility::Collapsed);
			return true;
		}
	}

	SkylineUIVar->ItemBox->ClearChildren();
	int Count = 0;
	for (TPair<TSubclassOf<UFGItemDescriptor>, int>& Pair : Minus) {
		UFSResourceIcon* Icon;
		if (Count < SkylineUIVar->ResourceIcon.Num()) {
			Icon = SkylineUIVar->ResourceIcon[Count];
		}
		else {
			TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(this, TEXT("/FactorySkyline/Widget_ResourceIcon.Widget_ResourceIcon_C"));
			Icon = CreateWidget<UFSResourceIcon>(SkylineUIVar, WidgetClass);
			SkylineUIVar->ResourceIcon.Add(Icon);
		}
		Icon->Load(Pair.Key, Pair.Value);
		SkylineUIVar->ItemBox->AddChild(Icon);
		Count++;
	}
	SkylineUIVar->ItemBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	*/
	return true;
}

bool AFSBuilder::Consume()
{
	AFSController* FSCtrlVar = (AFSController*)this->FSCtrl;
	if (FSCtrlVar->GetPlayer())
		return Inventory->Consume(FSCtrlVar->GetPlayer()->GetInventory(), &Cost);
	return Inventory->Consume(nullptr, &Cost);
}

void AFSBuilder::ScrollUp()
{
	if (this->Hologram)
		this->Hologram->Scroll(1);
}

void AFSBuilder::ScrollDown()
{
	if (this->Hologram)
		this->Hologram->Scroll(-1);
}

void AFSBuilder::onBuildFinish()
{
	//CurrentDesignMenu->ParentItem->AddChildAfter(NewDesignMenu, CurrentDesignMenu);
	//this->IsBuilding = false;
}


AFSBuilder::~AFSBuilder()
{
}

void UFSSyncBuild::Load()
{
	BuildableMapping.Empty();
	InternalConnectionMapping.Empty();
	List.Empty();
}

void UFSSyncBuild::Unload()
{
	NewDesignMenu = nullptr;
	BuildableMapping.Empty();
	InternalConnectionMapping.Empty();
	List.Empty();

	UFSBuildableOperatorList.Empty();
	passThroughBuildableList.Empty();
	passThroughNewList.Empty();

}

void UFSSyncBuild::PreWork()
{
	Step = 0;
	Current = 0;
	Index = 0;
	for (TWeakObjectPtr<AFGBuildable> BuildablePtr : this->BuildableSet) if (BuildablePtr.Get())
		List.Add(BuildablePtr);
	Total = List.Num();

	//UFSSelection::SetAutoRebuildTreeAll(false);
}

bool UFSSyncBuild::DoWork(float TimeLimitParam)
{
	this->TimeLimit = TimeLimitParam;
	Time.Start();
	while (Time.GetTime() < TimeLimitParam) {
		if (Step == 0) StepA();
		if (Step == 1) StepB();
		if (Step == 2) StepC();
		if (Step == 3) return true;
	}
	return false;
}

void UFSSyncBuild::StepA()
{
	//AFSkyline* FSkyline = AFSkyline::Get(this);

	//UFSBuildableOperatorList.Empty();
	//passThroughBuildableList.Empty();
	//passThroughNewList.Empty();

	for (; Index < List.Num(); Index++) {
		if (Time.GetTime() > TimeLimit) return;

		AFGBuildable* Buildable = List[Index].Get();
		if (Buildable) {
			UFSBuildableOperator* Operator = OperatorFactory->AcquireOperator(Buildable);
			if (Cast<AFGBuildableWire>(Buildable)) continue;
			
			//check if its a lift
			if (Cast<AFGBuildableConveyorLift>(Buildable)) {
				TArray< class AFGBuildablePassthrough* > ThroughsSource = Cast<AFGBuildableConveyorLift>(Buildable)->GetSnappedPassthroughs();
				// if its connected to any passthroughs we build these a little different so skip the normal buildig method.
				if (ThroughsSource[0] || ThroughsSource[1]) {
					continue;
				}
			}

			//if (!Operator->firstBuild) {
			//if (Buildable->GetName().Contains("Build_PowerPoleMk")) {
				AFGBuildable* NewBuildable = Operator->CreateCopy(FSTransform);
				if (NewBuildable != nullptr) {
					BuildableMapping.Add(Buildable, NewBuildable);

					if (NewBuildable) {

						Operator->UpdateInternelConnection(NewBuildable);
						//NewBuildable->PlayBuildEffects(FSkyline->FSCtrl->GetPlayer());
						//NewBuildable->ExecutePlayBuildEffects();

						this->NewDesign->BuildableSet.Add(NewBuildable);

						if (Cast<AFGBuildablePassthrough>(Buildable)) {
							UFSBuildableOperatorList.Add(Operator);
							passThroughBuildableList.Add(Cast<AFGBuildablePassthrough>(Buildable));
							passThroughNewList.Add(Cast<AFGBuildablePassthrough>(NewBuildable));
						}

					}
				}
			//}
		}
		Current++;
	}
	Index = 0;
	Step++;

	// conveyor buildablepassthrough logic here
	// first build all of the passthroughs

	for (int i = 0; i < passThroughBuildableList.Num(); i++) {
		AFGBuildablePassthrough* passThrough1 = passThroughBuildableList[i];
		UFGConnectionComponent* bottomConnection1 = passThrough1->mBottomSnappedConnection;
		UFGConnectionComponent* TopConnection1 = passThrough1->mTopSnappedConnection;

		// check if conveyor lift is connected to this through at the bottom
		if (bottomConnection1 != nullptr) {
			AFGBuildableConveyorLift* bottomConnectedLift1 = Cast<AFGBuildableConveyorLift>(Cast< UFGFactoryConnectionComponent>(bottomConnection1)->GetOuterBuildable());
			bool foundNoPassThroughConnections = true;
			int foundIndex = 0;
			for (int j = 0; j < passThroughBuildableList.Num(); j++) {
				// because we only want to look at other connections
				if (i == j) {
					continue;
				}

				AFGBuildablePassthrough* passThrough2 = passThroughBuildableList[j];
				UFGConnectionComponent* TopConnection2 = passThrough2->mTopSnappedConnection;

				if (!TopConnection2) {
					continue;
				}

				AFGBuildableConveyorLift* topConnectedLift1 = Cast<AFGBuildableConveyorLift>(Cast< UFGFactoryConnectionComponent>(TopConnection2)->GetOuterBuildable());
				if (bottomConnectedLift1 == topConnectedLift1) {
					foundNoPassThroughConnections = false;
					foundIndex = j;
					break;
				}
			}
			if (foundNoPassThroughConnections) {
				UFSBuildableOperator* Operator = OperatorFactory->AcquireOperator(Cast< AFGBuildable>(bottomConnectedLift1));
				Operator->bottomPassThrough = passThroughNewList[i];
				AFGBuildable* NewBuildable = Operator->CreateCopy(FSTransform);
				if (NewBuildable != nullptr) {
					BuildableMapping.Add(Cast< AFGBuildable>(bottomConnectedLift1), NewBuildable);
					Operator->UpdateInternelConnection(NewBuildable);
					this->NewDesign->BuildableSet.Add(NewBuildable);
				}
			}
		}
		////

		// check if conveyor lift is connected to this through at the top
		if (TopConnection1 != nullptr) {
			AFGBuildableConveyorLift* topConnectedLift1 = Cast<AFGBuildableConveyorLift>(Cast< UFGFactoryConnectionComponent>(TopConnection1)->GetOuterBuildable());
			bool foundNoPassThroughConnections = true;
			int foundIndex = 0;
			for (int j = 0; j < passThroughBuildableList.Num(); j++) {
				// because we only want to look at other connections
				if (i == j) {
					continue;
				}

				AFGBuildablePassthrough* passThrough2 = passThroughBuildableList[j];
				UFGConnectionComponent* bottomConnection2 = passThrough2->mBottomSnappedConnection;
				UFGConnectionComponent* TopConnection2 = passThrough2->mTopSnappedConnection;

				if (!bottomConnection2) {
					continue;
				}

				AFGBuildableConveyorLift* bottomConnectedLift4 = Cast<AFGBuildableConveyorLift>(Cast< UFGFactoryConnectionComponent>(bottomConnection2)->GetOuterBuildable());
				if (bottomConnectedLift4 == topConnectedLift1) {
					foundNoPassThroughConnections = false;
					foundIndex = j;
					break;
				}

				// its not connected to to anything else so this is the end of a piece
				if (bottomConnection2 == nullptr && TopConnection2 == nullptr) {
				}

			}
			if (foundNoPassThroughConnections) {
				UFSBuildableOperator* Operator = OperatorFactory->AcquireOperator(Cast< AFGBuildable>(topConnectedLift1));
				//Operator->bottomPassThrough = passThroughNewList[i];
				Operator->topPassThrough = passThroughNewList[i];
				AFGBuildable* NewBuildable = Operator->CreateCopy(FSTransform);
				if (NewBuildable != nullptr) {
					BuildableMapping.Add(Cast< AFGBuildable>(topConnectedLift1), NewBuildable);
					Operator->UpdateInternelConnection(NewBuildable);
					this->NewDesign->BuildableSet.Add(NewBuildable);
				}
			}

			// a top connect is connected to a bottom connection
			if (!foundNoPassThroughConnections) {

				// check if a connection between pass throughs has already been made if so we don't need to make duplicates
				///if (passThroughNewList[foundIndex]->mTopSnappedConnection != nullptr || passThroughNewList[i]->mBottomSnappedConnection != nullptr) {
					//continue;
				//}

				AFGBuildablePassthrough* passThrough2 = passThroughBuildableList[foundIndex];
				UFGConnectionComponent* bottomConnection2 = passThrough2->mBottomSnappedConnection;
				AFGBuildableConveyorLift* bottomConnectedLift = Cast<AFGBuildableConveyorLift>(Cast< UFGFactoryConnectionComponent>(bottomConnection2)->GetOuterBuildable());
				if (bottomConnectedLift == topConnectedLift1) {
					UFSBuildableOperator* Operator = OperatorFactory->AcquireOperator(Cast< AFGBuildable>(bottomConnectedLift));
					Operator->bottomPassThrough = passThroughNewList[i];
					Operator->topPassThrough = passThroughNewList[foundIndex];
					AFGBuildable* NewBuildable = Operator->CreateCopy(FSTransform);
					if (NewBuildable != nullptr) {
						BuildableMapping.Add(Cast< AFGBuildable>(topConnectedLift1), NewBuildable);
						Operator->UpdateInternelConnection(NewBuildable);
						this->NewDesign->BuildableSet.Add(NewBuildable);
					}
				}

			}
		}

	}

}

void UFSSyncBuild::StepB()
{
	for (; Index < List.Num(); Index++) {
		if (Time.GetTime() > TimeLimit) return;

		AFGBuildable* Buildable = List[Index].Get();
		if (Buildable) {
			UFSBuildableOperator* Operator = OperatorFactory->AcquireOperator(Buildable);

			if (Cast<AFGBuildableWire>(Buildable)) {
				//if (!Operator->firstBuild) {
					AFGBuildable* NewBuildable = Operator->CreateCopy(FSTransform);

					if (NewBuildable != nullptr) {
						BuildableMapping.Add(Buildable, NewBuildable);

						if (NewBuildable) {

							Operator->UpdateInternelConnection(NewBuildable);
							//NewBuildable->PlayBuildEffects(this->Player);
							//NewBuildable->ExecutePlayBuildEffects();

							this->NewDesign->BuildableSet.Add(NewBuildable);
						}
					}
					Current++;
				//}
			}
			else {
				AFGBuildable** NewBuildable = BuildableMapping.Find(Buildable);

				if (NewBuildable && *NewBuildable) {
					Operator->ApplySettingsTo(*NewBuildable);
					Operator->ApplyConnection(*NewBuildable, FSTransform, true);
				}
			}
		}
	}
	Index = 0;
	Step++;
}

void UFSSyncBuild::StepC()
{
	AFGBuildableSubsystem* BuildableSubsystem = AFGBuildableSubsystem::Get(this);
	AFGPipeSubsystem* PipeSubsystem = AFGPipeSubsystem::Get(GetWorld());
	AFGRailroadSubsystem* RailroadSubsystem = AFGRailroadSubsystem::Get(this);

	for (; Index < List.Num(); Index++) {
		AFGBuildable** Ptr = BuildableMapping.Find(List[Index].Get());
		if (Ptr) {
			IFGFluidIntegrantInterface* FluidIntegrant = Cast<IFGFluidIntegrantInterface>(*Ptr);
			if (FluidIntegrant) {
				PipeSubsystem->UnregisterFluidIntegrant(FluidIntegrant);
			}
		}
		
	}
	
	for (Index = 0; Index < List.Num(); Index++) {
		//if (Time.GetTime() > TimeLimit) return;
		AFGBuildable** Ptr = BuildableMapping.Find(List[Index].Get());
		if (Ptr) {
			AFGBuildableConveyorBase* Conveyor = Cast<AFGBuildableConveyorBase>(*Ptr);
			if (Conveyor) {
				BuildableSubsystem->AddConveyor(Conveyor);
				BuildableSubsystem->mConveyorBucketGroupsDirty = true;
			}
			AFGBuildableRailroadStation* Station = Cast<AFGBuildableRailroadStation>(*Ptr);
			if (Station) {
				RailroadSubsystem->AddTrainStation(Station);
				//SML::Logging::info(Station->GetTrackGraphID()); 
				//SML::Logging::info(Station->mStationIdentifier->GetTrackGraphID());
			}
			IFGFluidIntegrantInterface* FluidIntegrant = Cast<IFGFluidIntegrantInterface>(*Ptr);
			if (FluidIntegrant) {
				PipeSubsystem->RegisterFluidIntegrant(FluidIntegrant);
			}
			/*
			AFGBuildableRailroadTrack* Track = Cast<AFGBuildableRailroadTrack>(*Ptr);
			if (Track) {
				SML::Logging::info(Track->GetTrackGraphID());
				//RailroadSubsystem->AddTrack(Track);
			}*/
		}
	}
	BuildableSubsystem->mFactoryBuildingGroupsDirty = true;

	AFGBuildable** Result = BuildableMapping.Find(this->Anchor.Get());
	NewDesign->Anchor = Result ? *Result : nullptr;

	AFSkyline* Skyline = AFSkyline::Get(this);
	Skyline->SkylineUI->CompletionWidget->Load(CurrentDesignMenu, NewDesignMenu);
	Skyline->SkylineUI->CompletionWidget->StartEnterAnimation();

	//UFSSelection::SetAutoRebuildTreeAll(true);

	Step++;
}

int UFSSyncBuild::GetTotal()
{
	return Total;
}

int UFSSyncBuild::GetCurrent()
{
	return Current;
}
