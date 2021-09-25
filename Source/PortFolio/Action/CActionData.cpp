#include "CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "Managers/CBossManager.h"

void UCActionData::BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction)
{
	FTransform transform;

	TArray<ACAttachment*> Attachments;
	for (auto AttachmentClass : AttachmentClasses)
	{
		ACAttachment* Attachment = nullptr;
		if (!!AttachmentClass)
		{
			Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
			Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
			UGameplayStatics::FinishSpawningActor(Attachment, transform);
		}
		Attachments.Add(Attachment);
	}
	

	ACEquipment* Equipment = nullptr;
	if (!!EquipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter, "Equipmemt"));
		UGameplayStatics::FinishSpawningActor(Equipment, transform);

		for (auto Attachment : Attachments)
		{
			if (!!Attachment)
			{  
				Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
				Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
			}
		}
	}

	ACDoAction* DoAction = nullptr;

	if (!!DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		DoAction->SetDatas(DoActionDatas);
		for (auto Attachment : Attachments)
		{
			if (!!Attachment)
			{
				Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
				Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);
			}
		}
	}

	*OutAction = NewObject<UCAction>();
	for (int32 index = 0; index < Attachments.Num(); index++)
	{
		(*OutAction)->Attachments.Add(Attachments[index]);
	}

	(*OutAction)->Equipment = Equipment;
	(*OutAction)->DoAction = DoAction;
	(*OutAction)->EquipmentColor = EquipmentColor;
}

FString UCActionData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString str = "";
	str+=InOwnerCharacter->GetActorLabel();
	str+="_";
	str+=InName;
	str+="_";
	str+=GetName().Replace(L"DA",L"");

	return str;
}
