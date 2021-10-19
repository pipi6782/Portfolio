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

	//BehaviorComponent ��������
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);

	//�����׼� State/PatrolComponent ��������
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
		behavior->SetWaitMode();
		return;
	}

	CheckFalse(state->IsIdleMode());


	//Ÿ���� ������
	//������ ������ ���¸� ����
	
	bool bCanAttack = controller->CanAttack();

	if (bCanAttack)
	{
		controller->DisableAttack();
		behavior->SetActionMode();
		return;
	}

	//���� �÷��̾�� �Ÿ� ���
	float distance = enemy->GetDistanceTo(target);

	//�÷��̾ Strafe Range �ȿ� �ִٸ�
	if (distance < controller->GetStrafeRange())
	{
		behavior->SetStrafeMode();
		return;
	}

	//Ÿ���� �������� ���� �ִٸ�
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
