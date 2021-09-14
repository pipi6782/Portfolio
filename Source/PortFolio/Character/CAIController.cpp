#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

#include "Components/CBehaviorComponent.h"
#include "CEnemy_AI.h"
#include "CEnemy_Boss.h"
#include "CPlayer.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 45.0f;
	Sight->SetMaxAge(2.0f);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Sight);

	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing");
	Hearing->HearingRange = 600.0f;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = false;
	Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Hearing->SetMaxAge(2.0f);
	Perception->ConfigureSense(*Hearing);
	
	Perception->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	if (!!OwnerEnemy)
	{
		UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

		SetGenericTeamId(OwnerEnemy->GetTeamID());
		Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
		Behavior->SetBlackBoard(Blackboard);

		RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
	}
	else
	{
		OwnerBoss = Cast<ACEnemy_Boss>(InPawn);
		UseBlackboard(OwnerBoss->GetBehaviorTree()->BlackboardAsset, Blackboard);

		SetGenericTeamId(OwnerBoss->GetTeamID());
		Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
		Behavior->SetBlackBoard(Blackboard);

		RunBehaviorTree(OwnerBoss->GetBehaviorTree());
	}
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	ACPlayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		if (!!player)
		{
			break;
		}
	}
	Blackboard->SetValueAsObject("Player", player);
}

