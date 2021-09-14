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

	//��Ʈ�ѷ��� ������ �� ��üũ
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller,EBTNodeResult::Failed);

	//���� ������ �� ��üũ
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(enemy,EBTNodeResult::Failed);

	//�����Լ� PatrolComnponent�� ������ �� ��üũ
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

	//��Ʈ�ѷ��� ������ �� ��üũ
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//���� ������ �� ��üũ
	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(controller->GetPawn());
	if (enemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//�����Լ� PatrolComnponent�� ������ �� ��üũ
	UCPatrolComponent* patrol = CHelpers::GetComponent < UCPatrolComponent>(enemy);
	if (patrol == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//���� �������� ��ġ�� ���ؿ�
	FVector location = FVector::ZeroVector;
	float acceptance = 0.0f;
	patrol->GetMoveTo(location, acceptance);

	//��Ʈ�ѷ����� ���� �̵��϶�� ����� ����
	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptance, false);

	//�̵��� ����������
	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//�̵��� ����������
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		patrol->UpdateNextIndex();
	}
}
