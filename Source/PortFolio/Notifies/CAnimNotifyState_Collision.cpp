#include "CAnimNotifyState_Collision.h"
#include "Global.h"

#include "Components/CActionComponent.h"
#include "Action/CDoAction.h"
#include "Action/CAttachment.h"


FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction* doAction = action->GetCurrent()->GetDoAction();

	if (!!doAction)
	{
		for (ACAttachment* attachment : action->GetCurrent()->GetAttachments())
		{
			attachment->OnCollision();
		}
	}
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction* doAction = action->GetCurrent()->GetDoAction();

	if (!!doAction)
	{
		for (ACAttachment* attachment : action->GetCurrent()->GetAttachments())
		{
			if (attachment->GetName().Contains("Shield") == false)
			{
				attachment->OffCollision();
			}
		}
	}
}
