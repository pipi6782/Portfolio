#include "CBossManager.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"

#include "Character/CAIController.h"
#include "Character/CEnemy_Boss.h"
#include "Components/CStateComponent.h"

ACBossManager::ACBossManager()
{
	
}

void ACBossManager::BeginPlay()
{
	Super::BeginPlay();

	//모아야할 보스의 순서 세팅
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy_Boss::StaticClass(), actors);
	for (AActor* actor : actors)
	{
		CorrectBoss.Add(Cast<ACEnemy_Boss>(actor));
	}

	CorrectBoss.Sort();
	StartBossAttackTimer();
}

void ACBossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPrintTime) CLog::Print(AttackTime - RunningTime, 1);
	for (ACEnemy_Boss* boss : CorrectBoss)
	{
		ACAIController* controller = Cast<ACAIController>(boss->GetController());
		if (boss->GetName().Contains("First"))
		{
			CLog::Print(controller->CanAttack() ? "First true" : "First false", 2);
		}
		if (boss->GetName().Contains("Second"))
		{
			CLog::Print(controller->CanAttack() ? "Second true" : "Second false", 3);
		}
		if (boss->GetName().Contains("Third"))
		{
			CLog::Print(controller->CanAttack() ? "Third true" : "Third false", 4);
		}
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
	//보스가 셋다 뭉치고 맞는 순서면
	if (IsBossOrderingCorrect())
	{
		//TODO : Final 보스로 바뀌게 함
		Bosses[0]->UnfollowBoomerang();
		for (int32 i = 1; i < Bosses.Num(); i++)
		{
			Bosses[i]->AttachToComponent(Bosses[0]->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		}
	}
	else
	{
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
	Bosses.Empty();
}

void ACBossManager::OnBossAttackTimer()
{
	CLog::Print("Attack!!");
	for (ACEnemy_Boss* boss : CorrectBoss)
	{
		ACAIController* controller = Cast<ACAIController>(boss->GetController());
		if (!!controller)
		{
			controller->EnableAttack();
		}
	}
}

void ACBossManager::StartBossAttackTimer()
{
	//보스와 만나서 보스전이 시작됐을때만 사용
	UKismetSystemLibrary::K2_SetTimer(this, "OnBossAttackTimer",AttackTime,true);
}

