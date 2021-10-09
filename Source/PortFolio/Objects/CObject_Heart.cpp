#include "CObject_Heart.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/Character.h"

#include "Components/CStatusComponent.h"
ACObject_Heart::ACObject_Heart()
{
	PrimaryActorTick.bCanEverTick = true;

	//Mesh Setting
	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_Plane.SM_Plane'");
	Mesh->SetStaticMesh(mesh);

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset(&material, "MaterialInstanceConstant'/Game/Materials/MI_Heart.MI_Heart'");
	Mesh->SetMaterial(0, material);
	Mesh->SetRelativeLocation(FVector(0, 0, 75));
	Mesh->SetRelativeRotation(FRotator(0, 0, 90.0f));
	Mesh->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
	Mesh->SetCollisionProfileName("OverlapAllDynamic");
}

void ACObject_Heart::BeginPlay()
{
	Super::BeginPlay();
}

void ACObject_Heart::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator rotator = FRotator(0, 360, 0);

	AddActorWorldRotation(DeltaSeconds * rotator * RotationCycle);
}

void ACObject_Heart::Interact(AActor* InInteractor)
{
	CheckNull(InInteractor);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InInteractor);

	CheckNull(status);

	status->AddHealth(1.0f);

	Destroy();
}
