#include "CObject_Potion.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CInventoryComponent.h"
#include "Components/CStatusComponent.h"

void ACObject_Potion::Interact(AActor* InInteractor)
{
	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(InInteractor);

	CheckNull(inventory);

	//TODO : 아이템 추가 구현
	FSlotDesc desc = FSlotDesc();
	desc.Item = ItemDesc;
	desc.Quantity = 1;
	if(inventory->AddToInventory(desc))
	{
		Destroy();
	}
	else
	{

	}

}

void ACObject_Potion::UseItem()
{
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	status->AddHealth(PotionValue);
	Destroy();
}
