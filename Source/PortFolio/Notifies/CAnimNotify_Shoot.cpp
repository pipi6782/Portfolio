#include "CAnimNotify_Shoot.h"
#include "Global.h"

#include "Action/CThrow.h"

FString UCAnimNotify_Shoot::GetNotifyName_Implementation() const
{
    return "Shoot";
}

void UCAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    ACThrow* throwObject = nullptr;
    
    for (AActor* actor : MeshComp->GetOwner()->Children)
    {
        throwObject = Cast<ACThrow>(actor);
        if (!!throwObject) break;
    }

    CheckNull(throwObject);

    throwObject->StartThrowing();
}
