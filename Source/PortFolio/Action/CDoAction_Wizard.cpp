#include "CDoAction_Wizard.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"

#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Character/CEnemy.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "CAttachment.h"
#include "CThrow.h"

void ACDoAction_Wizard::BeginPlay()
{
	Super::BeginPlay();
}

ACDoAction_Wizard::ACDoAction_Wizard()
{
	CHelpers::GetAsset(&Particle, "ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_thunderBall.P_ky_thunderBall'");
}

void ACDoAction_Wizard::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		player->PlayMontage(&Datas[0]);
	}
	else
	{
		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	}
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoAction_Wizard::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;
	if (!!Particle)
	{
		ParticleComponent = UGameplayStatics::SpawnEmitterAttached(Particle, OwnerCharacter->GetMesh(), "Hand_Throw");
	}
}

void ACDoAction_Wizard::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Wizard::StartThrowing()
{
	FTransform transform = OwnerCharacter->GetMesh()->GetSocketTransform("Hand_throw");
	
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(OwnerCharacter->GetController());
	CheckNull(behavior);

	ACPlayer* player = behavior->GetTargetPlayer();

	transform.SetRotation(FQuat(UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), player->GetActorLocation())));
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>
		(
			Datas[0].ThrowClass,
			transform,
			OwnerCharacter
		);

	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Wizard::OnThrowBeginOverlap);

	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	if (!!ParticleComponent)
		ParticleComponent->DestroyComponent();
}

void ACDoAction_Wizard::OffParticle()
{
	if(!!ParticleComponent)
		ParticleComponent->SetVisibility(false); 
}

void ACDoAction_Wizard::OnThrowBeginOverlap(FHitResult InHitResult)
{
	CheckNull(InHitResult.Actor);
	CLog::Log(InHitResult.Actor->GetName());
	PrintLine();
	CheckTrue(InHitResult.Actor->IsA<ACEnemy>());
	CheckTrue(InHitResult.Actor->IsA<ACAttachment>());
	FDamageEvent e;
	InHitResult.Actor->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}