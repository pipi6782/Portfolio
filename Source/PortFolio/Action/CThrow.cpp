#include "CThrow.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/CBehaviorComponent.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CPlayer.h"

ACThrow::ACThrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ThrowEffect, "ThrowEffect",Sphere);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Sphere->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	Projectile->InitialSpeed = 2000.0f;
	Projectile->MaxSpeed = 2000.0f;
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->bRotationFollowsVelocity = true;

	InitialLifeSpan = 10.0f;
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckFalse(OtherActor->IsA<ACPlayer>());
	if (!HitEffect)
	{
		FTransform transform = HitEffectTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
	{
		OnThrowBeginOverlap.Broadcast(SweepResult);
	}

	Destroy();
}