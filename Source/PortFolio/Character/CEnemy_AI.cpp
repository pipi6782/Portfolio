#include "CEnemy_AI.h"
#include "Global.h"
#include "Components/CPatrolComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	CHelpers::CreateActorComponent<UCPatrolComponent>(this, &Patrol, "Patrol");
	CHelpers::GetClass(&AIControllerClass, "Blueprint'/Game/Enemies/AI/BP_CAIController.BP_CAIController_C'");
}