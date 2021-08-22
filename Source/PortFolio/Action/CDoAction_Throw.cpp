#include "CDoAction_Throw.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Character/CPlayer.h"
#include "Objects/CObject_Throw.h"

void ACDoAction_Throw::DoAction()
{
	CheckFalse(Datas.Num() > 0);
	Cast<ACPlayer>(OwnerCharacter)->ResetHitResult();
	if (!!Datas[0].AnimMontage)
	{
		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	}
	
}

void ACDoAction_Throw::Begin_DoAction()
{
	//Get Throw Object from player
	ACObject_Throw* Throw = nullptr;
	for (AActor* actor : OwnerCharacter->Children)
	{
		Throw = Cast<ACObject_Throw>(actor);
		if(!!Throw) break;
	}  
	
	CheckNull(Throw);
	Throw->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	if (Throw->OnObjectThrown.IsBound())
		Throw->OnObjectThrown.Broadcast();
}

void ACDoAction_Throw::End_DoAction()
{
	//End Action -> Set Idle, Set Move
	State->SetIdleMode();
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);
	action->SetSwordMode();
}