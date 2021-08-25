#include "CObject.h"
#include "Global.h"

ACObject::ACObject()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Scene, "Scene");

	Scene->Mobility = EComponentMobility::Movable;
}

void ACObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACObject::BeginPlay()
{
	Super::BeginPlay();
}

float ACObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
