#include "CAnimNotify_Shoot.h"
#include "Global.h"

#include"Components/CActionComponent.h"
#include "Action/CDoAction_Wizard.h"
#include "Action/CAction.h"

FString UCAnimNotify_Shoot::GetNotifyName_Implementation() const
{
    return "Shoot";
}

void UCAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
    CheckNull(action);

    ACDoAction_Wizard* wizard = Cast<ACDoAction_Wizard>(action->GetCurrent()->GetDoAction());
    CheckNull(wizard);

    wizard->StartThrowing();
}
