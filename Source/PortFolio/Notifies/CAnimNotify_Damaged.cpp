#include "CAnimNotify_Damaged.h"
#include "Global.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_Damaged::GetNotifyName_Implementation() const
{
	return "Damaged";
}

void UCAnimNotify_Damaged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);

	state->SetIdleMode();
}