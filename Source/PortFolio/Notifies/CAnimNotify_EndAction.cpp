#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Action/CAction.h"
#include "Action/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End Action";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetDoAction()->End_DoAction();
}
