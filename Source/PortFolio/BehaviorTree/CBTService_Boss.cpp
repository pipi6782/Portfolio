#include "CBTService_Boss.h"
#include "Global.h"

#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_Boss.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorComponent 가져오기
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	//적한테서 State/PatrolComponent 가져오기
	ACEnemy_Boss* enemy = Cast<ACEnemy_Boss>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(state);

	CheckTrue(state->IsDeadMode());

<<<<<<< HEAD
	if (enemy->GetName().Contains("Final"))
	{
		if (controller->CanRunTree() == true)
		{
			if (bStartedSpawn == false)
			{
				UKismetSystemLibrary::K2_SetTimer(this, "StartSpawn", SpawnTime, true);
				bStartedSpawn = true;
				return;
			}
		}
		if (bCanSpawn)
		{
			bCanSpawn = false;
			behavior->SetSpawnMode();
			return;
		}
	}

=======
>>>>>>> parent of 8268443 (UE_2021_10_18_Final)
	//공격을 당했을때
	if (state->IsDamagedMode())
	{
		behavior->SetDamagedMode();
		return;
	}

	//타겟이 없을떄
	ACPlayer* target = behavior->GetTargetPlayer();

	if (target == nullptr)
	{
		behavior->SetWaitMode();
		return;
	}

	CheckFalse(state->IsIdleMode());


	//타겟이 있을때
	//공격이 가능한 상태면 공격
	
	bool bCanAttack = controller->CanAttack();

	if (bCanAttack)
	{
		controller->DisableAttack();
		behavior->SetActionMode();
		return;
	}

	//적과 플레이어간의 거리 계산
	float distance = enemy->GetDistanceTo(target);

	//플레이어가 Strafe Range 안에 있다면
	if (distance < controller->GetStrafeRange())
	{
		behavior->SetStrafeMode();
		return;
	}

	//타겟이 감지범위 내에 있다면
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}

void UCBTService_Boss::StartSpawn()
{
	bCanSpawn = true;
}
