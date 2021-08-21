#include "CObject_Throw.h"
#include "Global.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACObject_Throw::ACObject_Throw()
	:ACObject()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");
	CHelpers::CreateComponent(this, &Sphere, "Sphere", Scene);
	CHelpers::CreateComponent(this, &Destructible, "Destructible", Scene);

	UDestructibleMesh* mesh;
	CHelpers::GetAsset(&mesh, "DestructibleMesh'/Game/Meshes/DM_Urn.DM_Urn'");
	Destructible->SetDestructibleMesh(mesh);
	Destructible->Mobility = EComponentMobility::Movable;

}

void ACObject_Throw::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PrintLine();
}

void ACObject_Throw::BeginPlay()
{
	Super::BeginPlay();
}

void ACObject_Throw::Begin_Interact(ACharacter* InCharacter)
{
}

void ACObject_Throw::OnThrown()
{
}
