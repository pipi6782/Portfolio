#include "CBTService_Strafe.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "Character/CPlayer.h"
#include "Character/CEnemy_Boss.h"
#include "Character/CAIController.h"
#include "Components/CBehaviorComponent.h"

UCBTService_Strafe::UCBTService_Strafe()
{
	NodeName = "Strafe";
	bNotifyTick = true;
}

void UCBTService_Strafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(controller->GetPawn());
	UBlackboardComponent* blackBoard = controller->GetBlackboardComponent();

	CheckNull(controller);
	CheckNull(boss);
	CheckNull(blackBoard);

	UEnvQuery* query = nullptr;
	GetStrafeQuery(controller, query);
	CheckNull(query);

	//Ÿ������ ���� �÷��̾ ������������ Behavior Component�� ������
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);
	controller->SetFocus(behavior->GetTargetPlayer());

	FEnvQueryRequest moveToLocationResult = FEnvQueryRequest(query, boss);
	moveToLocationResult.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &UCBTService_Strafe::MoveToStrafePoint);

}

void UCBTService_Strafe::GetStrafeQuery(ACAIController* InBossController, UEnvQuery*& OutQuery)
{
	//���Ͻ�ų eqs����
	OutQuery = nullptr;

	//PlayerKey���� ���� �������� null�̸� �Լ� ����
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(InBossController->GetPawn());
	CheckNull(boss);

	UBlackboardComponent* blackBoard = InBossController->GetBlackboardComponent();
	CheckNull(blackBoard);
	AActor* actor = Cast<AActor>(blackBoard->GetValueAsObject("Player"));
	CheckNull(actor);
	FVector rightNormalVector = UKismetMathLibrary::Normal(actor->GetActorRightVector());
	FVector normalVectorToBoss = UKismetMathLibrary::Normal(UKismetMathLibrary::GetDirectionUnitVector(actor->GetActorLocation(), boss->GetActorLocation()));
	float dotResult = FVector::DotProduct(rightNormalVector, normalVectorToBoss);

	//TODO : eqs ���� ���� �� ���� ��� �ֱ�
	if (dotResult <= 0)
	{
		CHelpers::GetAssetDynamic<UEnvQuery>(&OutQuery, "EnvQuery'/Game/Enemies/Boss/EnvQuery/EQS_StrafeRight.EQS_StrafeRight'");
	}
	else
	{
		CHelpers::GetAssetDynamic<UEnvQuery>(&OutQuery, "EnvQuery'/Game/Enemies/Boss/EnvQuery/EQS_StrafeLeft.EQS_StrafeLeft'");
	}

}

void UCBTService_Strafe::MoveToStrafePoint(TSharedPtr<FEnvQueryResult> result)
{
	//EQS����� ���и� �Լ� ��
	CheckFalse(result->IsSuccsessful());
	
	//������ ������
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(result->Owner);
	CheckNull(boss);

	//��Ʈ�ѷ��� ������, ��Ʈ�ѷ��� ������ �Լ� ��
	ACAIController* controller = Cast<ACAIController>(boss->GetController());
	CheckNull(controller);

	//EQS����� ���� ù��° ����� ������, (0,0,0)�� ������ �Լ� ����
	FVector resultLocation = result->GetItemAsLocation(0);
	CheckTrue(FMath::IsNearlyZero(FVector::Dist(resultLocation, FVector::ZeroVector)));
	
	//��Ʈ�ѷ��� ���������� ���������� ������ �ش� �������� �̵�
	controller->MoveToLocation(resultLocation, 5.0f, false);
}
