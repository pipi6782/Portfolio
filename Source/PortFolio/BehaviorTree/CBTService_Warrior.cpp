#include "CBTService_Warrior.h"
#include "Global.h"

#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"

UCBTService_Warrior::UCBTService_Warrior()
{
	NodeName = "Melee";
}

void UCBTService_Warrior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorComponent 가져오기
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	//적한테서 State/PatrolComponent 가져오기
	ACEnemy* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(enemy);

	CheckNull(state);
	CheckNull(patrol);

	CheckTrue(state->IsDeadMode());

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
		if (!!patrol && patrol->IsValid())
		{
			behavior->SetPatrolMode();
			return;
		}

		behavior->SetWaitMode();
		return;
	}
	//타겟이 있을때
	else
	{
		//타겟이 죽은 상태거나 숨는 구역에 있을 경우
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode() || target->IsHiding())
		{
			behavior->SetPatrolMode();
			controller->ClearFocus(EAIFocusPriority::Gameplay);
			return;
		}
	}

	//적과 플레이어간의 거리 계산
	float distance = enemy->GetDistanceTo(target);

	//플레이어가 사정거리 안에 있다면
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	//타겟이 감지범위 내에 있다면
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}
