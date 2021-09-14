#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage(Datas[0].AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOtherActor)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherActor);

	CheckNull(InOtherActor);

	//이미 타격한 캐릭터는 무시함
	for (const AActor* actor : DamagedActor)
	{
		if (InOtherActor == actor)
		{
			return;
		}
	}

	DamagedActor.Add(InOtherActor);

	//HitStop
	float hitStop = Datas[0].HitStop;
	FTimerHandle WaitHandle;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		GetWorld()->GetTimerManager().SetTimer
		(
			WaitHandle,
			FTimerDelegate::CreateLambda([&]() //[&] 이곳에 들어오는 함수들은 전부 다 참조변수이다, 내부에서 수정 시 영향을 미침
				{
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
				}),
			hitStop * 2e-2f,
			false
			);
	}

	//SendDamage
	FDamageEvent e;
	InOtherActor->TakeDamage(Datas[0].Power, e, InAttacker->GetInstigatorController(), InAttackCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOtherActor)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherActor);

	DamagedActor.Empty();
}
