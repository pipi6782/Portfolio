#include "CObject_Potion_Yellow.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"

#include"Character/CPlayer.h"

ACObject_Potion_Yellow::ACObject_Potion_Yellow()
{
	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Potions_pack/CombinedMeshes/SM_flask_01.SM_flask_01'");
	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	Mesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	Mesh->SetCollisionProfileName("OverlapAllDynamic");
}

void ACObject_Potion_Yellow::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACObject_Potion_Yellow::OnComponentBeginOverlap);
}

void ACObject_Potion_Yellow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	//player->AddItem(this);

	if (!!Particle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation() + Mesh->GetRelativeLocation());

	Destroy();
}
