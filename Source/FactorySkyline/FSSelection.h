// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "FGOutlineComponent.h"
#include "FGColoredInstanceMeshProxy.h"
#include "Buildables/FGBuildable.h"
#include "FSDesign.h"
#include "FSSelectService.h"
#include "FSSelection.generated.h"


struct FSMeshMaterial
{
	TWeakObjectPtr<UMeshComponent> MeshComponent = nullptr;
	UFGColoredInstanceMeshProxy* MeshProxy = nullptr;
	TArray<UMaterialInterface*> MaterialInterfaceList;
	TArray<UMaterialInterface*> MaterialInterfaceList2;
	//TMap< TWeakObjectPtr<UMeshComponent>, TArray<UMaterialInterface*> > MeshComponentArray;
};

struct FSActorMaterial
{
	bool Init = false;
	TWeakObjectPtr<AFGBuildable> Buildable = nullptr;
	TArray<FSMeshMaterial> MeshList;
};

struct FSMaterialHandle
{
	//UMaterialInterface* Material;
	TArray<class UMaterialInterface*> Materials;
	TArray<UMaterialInterface*> MaterialInterfaceList;
	TMap<UMeshComponent*, FSMeshMaterial* > MaterialMapping;
	UMaterialInterface* Material;
	int32 Handle;
};

struct FSISMNode
{
	UInstancedStaticMeshComponent* ISMComponent = nullptr;
	TArray<FSMaterialHandle*> Handles;
};

/**
 * 
 */

USTRUCT()
struct FACTORYSKYLINE_API FSavedMaterialInterfaces
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<class UMaterialInterface*> MaterialInterfaces;

public:
	FORCEINLINE ~FSavedMaterialInterfaces() = default;
};

USTRUCT()
struct FACTORYSKYLINE_API FSelectedActorInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<class UMeshComponent*, FSavedMaterialInterfaces> SavedMaterialInterfaces;

public:
	FORCEINLINE ~FSelectedActorInfo() = default;
};


USTRUCT()
struct FACTORYSKYLINE_API FComponentSavedInterfaces
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UMaterialInterface*> SavedInterfaces;

public:
	FORCEINLINE ~FComponentSavedInterfaces() = default;
};

UCLASS()
class FACTORYSKYLINE_API UFSSelection : public UObject
{
	GENERATED_BODY()
public:

	void Init();

	void Select(AFGBuildable* Buildable);
	bool ConnectSelect(AFGBuildable* Buildable);
	bool ConnectSelectCheckReady();
	bool RectSelectStart(const FVector2D& Start, bool SelectPositive);
	bool RectSelectCheckReady();
	bool RectSelectUpdate(FVector2D Cursor);
	void RectSelectExit(bool Valid);

	void Load(UFSDesign* DesignParam);
	void LoadSelect(UFSDesign* Design);
	void LoadSetAnchor(UFSDesign* Design);
	static void SetAutoRebuildTreeAll(bool Auto);
	void Unload();
	void BeginDestroy();

	bool IsHightLight(AFGBuildable* Buildable);
	void EnableHightLight(AFGBuildable* Buildable);
	void EnableHightLight(AFGBuildable* Buildable, UMaterialInterface* Material);
	void EnableHightLight(FSActorMaterial& Cache, AFGBuildable* Buildable, UMaterialInterface* Material);
	void DisableHightLight(AFGBuildable* Buildable);
	void EnableHightLightFocus(AFGBuildable* Buildable);
	void DisableHightLightFocus();
	void DisableAll();
	
	FSISMNode* GetISM(UFGColoredInstanceMeshProxy* MeshProxy, UMaterialInterface* Material);
	void AddInstance(UFGColoredInstanceMeshProxy* MeshProxy, UMaterialInterface* Material);
	void RemoveInstance(UFGColoredInstanceMeshProxy* MeshProxy, FSMaterialHandle* Node);
	void AddInstance(UFGColoredInstanceMeshProxy* MeshProxy, uint8 Slot);
	void RemoveInstance(UFGColoredInstanceMeshProxy* MeshProxy, uint8 Slot);

	void SetMeshInstanced(UMeshComponent* MeshComp, bool Instanced) const;

	void InitMaterials();

	void HideHologram(AActor* Actor, FSMaterialHandle& ActorInfo);

	UFSDesign* Design;
	UFSConnectSelectService* ConnectSelectService = nullptr;
	UFSRectSelectService* RectSelectService = nullptr;
	AFGBuildable* CurrentFocus = nullptr;

	int LastSelectMode;
	TMap<TWeakObjectPtr<AFGBuildable>, int> RectSelectBuffer;

	TMap<AFGBuildable*, FSActorMaterial> ActorMaterialCache;

	TMap<AFGBuildable*, FSActorMaterial> ActorMaterialCache2;

	UPROPERTY()
	UMaterialInstanceConstant* Hologram = nullptr;

	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> GCFlag;
	TMap<TPair<UFGColoredInstanceManager*, UMaterialInterface* >, FSISMNode*> ISMMapping;
	//TMap<UMeshComponent*, FSMaterialHandle > MaterialMapping;
	TMap<UMeshComponent*, FSMaterialHandle* > MaterialMapping;
	TMap<AActor*, FSMaterialHandle*> SelectedMap;
	TMap<UMeshComponent*, FSMaterialHandle* > MaterialMapping2;

	UPROPERTY()
	TSet<UMaterialInstanceDynamic*> DynamicInstanceSet;

	UPROPERTY()
	UMaterialInstanceDynamic* SelectMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* FocusMaterial;

	UPROPERTY()
	UTexture* Scanline;

	UPROPERTY()
	bool MaterialisInitialized;

	//UPROPERTY()
	//TMap<AActor*, FSelectedActorInfo> SelectedMap;

	//UPROPERTY()
	//TMap<AActor*, TArray<UMaterialInterface*>> SavedInterfaceList;

	//UPROPERTY()
	//TMap<UMeshComponent*, FComponentSavedInterfaces> SavedMaterialInterfaces;

};
