#include "CMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontageComponent::UCMontageComponent()
{

}


// Called when the game starts
void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get Rows from DataTable
	TArray<FMontageData*> datas;

	DataTable->GetAllRows<FMontageData>("", datas);

	//Write Datas from Array
	for (int32 i = 0; i < static_cast<int32>(EStateType::Max); i++)
	{
		for (FMontageData* data : datas)
		{
			if (static_cast<EStateType>(i) == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}
	}
}

void UCMontageComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	FMontageData* data = Datas[static_cast<int32>(InStateType)];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}


void UCMontageComponent::PlayDead()
{
	//PlayAnimMontage(EStateType::Dead)
}

void UCMontageComponent::PlayDamaged()
{
	PlayAnimMontage(EStateType::Damaged);
}