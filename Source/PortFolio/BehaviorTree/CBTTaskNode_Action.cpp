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

	//컨트롤러와 적을 가져온 후 널체크
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Failed);

	//플레이어를 가져오기 위해 Behavior Component를 가져온 후 타켓으로 설정된 플레이어를 가져옴
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNullResult(behavior, EBTNodeResult::Failed);
	ACPlayer* player = behavior->GetTargetPlayer();
	CheckNullResult(player, EBTNodeResult::Failed);

	//적의 이동을 멈추고 플레이어를 바라보게함
	controller->StopMovement();
	controller->SetFocus(player);

	//적에게서 Action Component를 가져온 후 널체크
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNullResult(action, EBTNodeResult::Failed);

	RunningTime = 0.0f;
	action->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	//컨트롤러와 적을 가져온 후 널체크
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

	//적에게서 State Component를 가져온 후 널체크
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
	if (state == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	RunningTime += DeltaSeconds;

	//공격이 끝나고 Idle Mode가 되고, 지정된 시간이상이 흘렀으면 해당 Task 종료
	if (state->IsIdleMode() && RunningTime >= Delay)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
