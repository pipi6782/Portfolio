#include "CFloor.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Character/CPlayer.h"

ACFloor::ACFloor()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &Mesh, "Mesh",Scene);
	CHelpers::CreateComponent(this, &Box, "Box",Scene);

	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_Plane.SM_Plane'");
	Mesh->SetStaticMesh(mesh);

	Mesh->SetRelativeScale3D(FVector(5, 5, 1));

	Box->SetRelativeLocation(FVector(0, 0, 30));
	Box->SetBoxExtent(FVector(250, 250, 32));

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset(&material, "MaterialInstanceConstant'/Game/Materials/MI_Force.MI_Force'");
	Mesh->SetMaterial(0, material);
}

void ACFloor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACFloor::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACFloor::OnComponentEndOverlap);
}

void ACFloor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckNull(Cast<ACPlayer>(OtherActor));
	
	ACPlayer* player = Cast<ACPlayer>(OtherActor);

	player->EnableHidden();
}

void ACFloor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckNull(Cast<ACPlayer>(OtherActor));

	ACPlayer* player = Cast<ACPlayer>(OtherActor);

	player->DisableHidden();
}
