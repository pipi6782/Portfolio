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

	Projectile->InitialSpeed = 0.0f;
	Projectile->MaxSpeed = 0.0f;
	Projectile->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = 10.0f;
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
}

void ACThrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector scale = Sphere->GetRelativeScale3D();
	scale = UKismetMathLibrary::VInterpTo(scale, DesiredScale, DeltaTime,1/DesiredTime);

	Sphere->SetRelativeScale3D(scale);

	UKismetSystemLibrary::DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 200, FLinearColor::Black);
}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());
	CheckTrue(OtherActor->GetClass() == GetClass());
	CheckTrue(OtherActor->GetClass() == GetOwner()->GetClass());
	if (!HitEffect)
	{
		FTransform transform = HitEffectTransform;
		transform.AddToTranslation(GetActorLocation());
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

void ACThrow::StartThrowing()
{
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(GetOwner());

	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(boss->GetController());
	if (!!behavior)
	{
		FRotator temp = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), behavior->GetTargetPlayer()->GetActorLocation());
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), behavior->GetTargetPlayer()->GetActorLocation()));
	}
	Projectile->SetVelocityInLocalSpace(GetActorForwardVector() * 2000.0f);
}

