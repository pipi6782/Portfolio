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

	//타겟으로 잡은 플레이어를 가져오기위해 Behavior Component를 가져옴
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
	CheckNull(behavior);
	controller->SetFocus(behavior->GetTargetPlayer());

	FEnvQueryRequest moveToLocationResult = FEnvQueryRequest(query, boss);
	moveToLocationResult.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &UCBTService_Strafe::MoveToStrafePoint);

}

void UCBTService_Strafe::GetStrafeQuery(ACAIController* InBossController, UEnvQuery*& OutQuery)
{
	//리턴시킬 eqs세팅
	OutQuery = nullptr;

	//PlayerKey에서 값을 가져오고 null이면 함수 끝냄
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(InBossController->GetPawn());
	CheckNull(boss);

	UBlackboardComponent* blackBoard = InBossController->GetBlackboardComponent();
	CheckNull(blackBoard);
	AActor* actor = Cast<AActor>(blackBoard->GetValueAsObject("Player"));
	CheckNull(actor);
	FVector rightNormalVector = UKismetMathLibrary::Normal(actor->GetActorRightVector());
	FVector normalVectorToBoss = UKismetMathLibrary::Normal(UKismetMathLibrary::GetDirectionUnitVector(actor->GetActorLocation(), boss->GetActorLocation()));
	float dotResult = FVector::DotProduct(rightNormalVector, normalVectorToBoss);

	//TODO : eqs 쿼리 생성 후 에셋 경로 넣기
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
	//EQS결과가 실패면 함수 끝
	CheckFalse(result->IsSuccsessful());
	
	//보스를 가져옴
	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(result->Owner);
	CheckNull(boss);

	//컨트롤러를 가져옴, 컨트롤러가 없으면 함수 끝
	ACAIController* controller = Cast<ACAIController>(boss->GetController());
	CheckNull(controller);

	//EQS결과중 제일 첫번째 결과만 가져옴, (0,0,0)이 나오면 함수 종료
	FVector resultLocation = result->GetItemAsLocation(0);
	CheckTrue(FMath::IsNearlyZero(FVector::Dist(resultLocation, FVector::ZeroVector)));
	
	//컨트롤러를 성공적으로 가져왔으니 보스를 해당 방향으로 이동
	controller->MoveToLocation(resultLocation, 5.0f, false);
}
