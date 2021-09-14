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

	//BehaviorComponent ��������
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	//�����׼� State/PatrolComponent ��������
	ACEnemy* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(enemy);

	CheckNull(state);
	CheckNull(patrol);

	CheckTrue(state->IsDeadMode());

	//������ ��������
	if (state->IsDamagedMode())
	{
		behavior->SetDamagedMode();
		return;
	}

	//Ÿ���� ������
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
	//Ÿ���� ������
	else
	{
		//Ÿ���� ���� ���°ų� ���� ������ ���� ���
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode() || target->IsHiding())
		{
			behavior->SetPatrolMode();
			controller->ClearFocus(EAIFocusPriority::Gameplay);
			return;
		}
	}

	//���� �÷��̾�� �Ÿ� ���
	float distance = enemy->GetDistanceTo(target);

	//�÷��̾ �����Ÿ� �ȿ� �ִٸ�
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	//Ÿ���� �������� ���� �ִٸ�
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}
