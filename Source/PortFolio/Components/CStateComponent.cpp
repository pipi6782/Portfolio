#include "CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}
void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}
void UCStateComponent::SetRollingMode()
{
	ChangeType(EStateType::Rolling);
}
void UCStateComponent::SetDamagedMode()
{
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetInteractingMode()
{
	ChangeType(EStateType::Interacting);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InNewType);
}

