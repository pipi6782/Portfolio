#include "CBTTaskNode_Speed.h"
#include "Global.h"

#include "Character/CEnemy.h"
#include "Character/CAIController.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy* enemy = Cast<ACEnemy>(controller->GetPawn());

	CheckNullResult(controller, EBTNodeResult::Failed);
	CheckNullResult(enemy,EBTNodeResult::Failed);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(enemy);

	CheckNullResult(status, EBTNodeResult::Failed);

	status->SetSpeed(Type);

	return EBTNodeResult::Succeeded;
}