#include "CBossManager.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_Boss.h"
#include "Components/CStateComponent.h"

ACBossManager::ACBossManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACBossManager::BeginPlay()
{
	Super::BeginPlay();

	//��ƾ��� ������ ���� ����
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy_Boss::StaticClass(), actors);
	for (AActor* actor : actors)
	{
		CorrectBoss.Add(Cast<ACEnemy_Boss>(actor));
	}

	CorrectBoss.Sort();

	bEnableTick = true;
}

void ACBossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFalse(bEnableTick);
	RunningTime += DeltaTime;
	CLog::Print(AttackTime - RunningTime, 0);
	CLog::Print(Bosses.Num(), 1);
	if (RunningTime >= AttackTime)
	{
		for (ACEnemy_Boss* boss : CorrectBoss)
		{
			ACAIController* controller = Cast<ACAIController>(boss->GetController());
			if (!!controller)
			{
				controller->EnableAttack();
			}
		}
		RunningTime = 0.0f;
	}
}

void ACBossManager::AddBoss(ACEnemy_Boss* InBoss)
{
	CheckNull(InBoss);
	CheckTrue(Bosses.Contains(InBoss));
	Bosses.Add(InBoss);
}

void ACBossManager::CheckBossOnBoomerangEnd()
{
	CLog::Log("Check");
	//������ �´� ��ġ�� �´� ������
	if (IsBossOrderingCorrect())
	{
		//TODO : Final ������ �ٲ�� ��
	}
	else
	{
		if (Bosses.Num() == 3) CLog::Print("Three");
		for (int32 i = 0; i < Bosses.Num(); i++)
		{
			UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Bosses[i]);
			if (!!state)
			{
				state->SetIdleMode();
			}
			Bosses[i]->UnfollowBoomerang();
		}
	}
}

bool ACBossManager::IsBossOrderingCorrect()
{
	CheckTrueResult(Bosses.Num() < 3, false);
	for (int32 i = 0; i < Bosses.Num(); i++)
	{
		if (Bosses[i] != CorrectBoss[i])
		{
			return false;
		}
	}
	return true;
}

void ACBossManager::Reset()
{
	CLog::Log("Reset");
	Bosses.Empty();
}

void ACBossManager::SetBossAttackTimer()
{
	//������ ������ �������� ���۵������� ���
	bEnableTick = true;
}

