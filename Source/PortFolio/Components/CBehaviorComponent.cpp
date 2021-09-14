#include "CBehaviorComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBehaviorComponent::UCBehaviorComponent()
{

}


void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol;
}

bool UCBehaviorComponent::IsDamagedMode()
{
	return GetType() == EBehaviorType::Damaged;
}

bool UCBehaviorComponent::IsStrafeMode()
{
	return GetType() == EBehaviorType::Strafe;
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}

void UCBehaviorComponent::SetDamagedMode()
{
	ChangeType(EBehaviorType::Damaged);
}

void UCBehaviorComponent::SetStrafeMode()
{
	ChangeType(EBehaviorType::Strafe);
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorComponent::GetWarpLocation()
{
	return Blackboard->GetValueAsVector(WarpLocationKey);
}


void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	EBehaviorType prev = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(prev, InType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}
