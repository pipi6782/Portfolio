#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Action/CActionData.h"
#include "Action/CEquipment.h"
#include "Action/CDoAction.h"
#include "Action/CAttachment.h"
#include "Action/CAction.h"

UCActionComponent::UCActionComponent()
{
}


// Called when the game starts
void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	CheckNull(character);

	for (int32 i = 0; i < static_cast<int32>(EActionType::Max); i++)
	{
		if (!!DataAssets[i])
		{
			DataAssets[i]->BeginPlay(character, &Datas[i]);
		}
	}
}



void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[static_cast<int32>(Type)])
		Datas[static_cast<int32>(Type)]->GetEquipment()->Unequip();

	if (!!Datas[static_cast<int32>(EActionType::Unarmed)])
		Datas[static_cast<int32>(EActionType::Unarmed)]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetSwordMode()
{
	SetMode(EActionType::Sword);
}

void UCActionComponent::SetThrowingMode()
{
	SetMode(EActionType::Throwing);
}
void UCActionComponent::SetBoomerangMode()
{
	SetMode(EActionType::Boomerang);
}

void UCActionComponent::SetWizardMode()
{
	SetMode(EActionType::Wizard);
}

void UCActionComponent::DoAction()
{
	if (IsUnarmedMode())
	{

		SetSwordMode();
	}
	//TODO : DoAction구현 후 Action실행
	if (!!Datas[static_cast<int32>(Type)])
	{
		ACDoAction* doAction = Datas[static_cast<int32>(Type)]->GetDoAction();

		if (!!doAction)
		{
			doAction->DoAction();
		}
	}
}

void UCActionComponent::Dead()
{
	OffAllCollision();
}

void UCActionComponent::End_Dead()
{
	//TODO : 죽었을 때 플레이어와 적에 따라 다른 액션 구현
}

void UCActionComponent::AbortByDamage()
{

}

void UCActionComponent::OffAllCollision()
{
	for (UCAction* data : Datas)
	{
		if (!!data == false)
			continue;

		for (ACAttachment* attachment : data->GetAttachments())
		{
			if (!!attachment == false) continue;
		}

		for (ACAttachment* attachment : data->GetAttachments())
		{
			attachment->OffCollision();
		}
	}
}


void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	if (InType == EActionType::Boomerang && Datas[static_cast<int32>(InType)]->GetDoAction()->GetAction() == true)
	{
		return;
	}
	if (IsUnarmedMode() == false)
	{
		if (!!Datas[static_cast<int32>(Type)] && Datas[static_cast<int32>(Type)]->GetDoAction()->GetAction() == false)
			Datas[static_cast<int32>(Type)]->GetEquipment()->Unequip();
	}

	if (!!Datas[static_cast<int32>(InType)] && !!Datas[static_cast<int32>(InType)]->GetEquipment())
	{
		Datas[static_cast<int32>(InType)]->GetEquipment()->Equip();
	}

	//Equip
	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}