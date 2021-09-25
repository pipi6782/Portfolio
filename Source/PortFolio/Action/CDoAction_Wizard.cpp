#include "CDoAction_Wizard.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Character/CEnemy.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CThrow.h"

void ACDoAction_Wizard::BeginPlay()
{
	Super::BeginPlay();
}

void ACDoAction_Wizard::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoAction_Wizard::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>
		(
			Datas[0].ThrowClass,
			transform,
			OwnerCharacter,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);


	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Wizard::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	throwObject->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Throw");
}

void ACDoAction_Wizard::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Wizard::OnThrowBeginOverlap(FHitResult InHitResult)
{
	CheckTrue(InHitResult.Actor->IsA<ACEnemy>());
	FDamageEvent e;
	InHitResult.Actor->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}