#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Character/CAIController.h"
#include "Character/CEnemy_AI.h"
#include "Components/CPatrolComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//컨트롤러를 가져온 후 널체크
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller,EBTNodeResult::Failed);

	//적을 가져온 후 널체크
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy,EBTNodeResult::Failed);

	//적에게서 PatrolComnponent를 가져온 후 널체크
	UCPatrolComponent* patrol = CHelpers::GetComponent < UCPatrolComponent>(enemy);
	CheckNullResult(patrol,EBTNodeResult::Failed);

	FVector location = FVector::ZeroVector;
	float acceptance = 0.0f;

	if (patrol->GetMoveTo(location, acceptance) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//컨트롤러를 가져온 후 널체크
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//적을 가져온 후 널체크
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	if (enemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//적에게서 PatrolComnponent를 가져온 후 널체크
	UCPatrolComponent* patrol = CHelpers::GetComponent < UCPatrolComponent>(enemy);
	if (patrol == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//다음 목적지의 위치를 구해옴
	FVector location = FVector::ZeroVector;
	float acceptance = 0.0f;
	patrol->GetMoveTo(location, acceptance);

	//컨트롤러에게 직접 이동하라는 명령을 내림
	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptance, false);

	//이동에 실패했을때
	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//이동에 성공했을때
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		patrol->UpdateNextIndex();
	}
}
