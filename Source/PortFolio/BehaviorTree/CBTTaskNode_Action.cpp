#include "CBTTaskNode_Action.h"
#include "Global.h"

#include "Character/CPlayer.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//��Ʈ�ѷ��� ���� ������ �� ��üũ
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	//�÷��̾ �������� ���� Behavior Component�� ������ �� Ÿ������ ������ �÷��̾ ������
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNullResult(behavior, EBTNodeResult::Failed);
	ACPlayer* player = behavior->GetTargetPlayer();
	CheckNullResult(player, EBTNodeResult::Failed);

	//���� �̵��� ���߰� �÷��̾ �ٶ󺸰���
	controller->StopMovement();
	controller->SetFocus(player);

	//�����Լ� Action Component�� ������ �� ��üũ
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNullResult(action, EBTNodeResult::Failed);

	RunningTime = 0.0f;
	action->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	//��Ʈ�ѷ��� ���� ������ �� ��üũ
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	if (enemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//�����Լ� State Component�� ������ �� ��üũ
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	if (state == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	RunningTime += DeltaSeconds;

	//������ ������ Idle Mode�� �ǰ�, ������ �ð��̻��� �귶���� �ش� Task ����
	if (state->IsIdleMode() && RunningTime >= Delay)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
