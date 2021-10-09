#include "CUserWidget_Inventory.h"
#include "Global.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

#include "Components/CInventoryComponent.h"
#include "Objects/CObject_Potion.h"
#include "CUserWidget_Item.h"


void UCUserWidget_Inventory::NativeConstruct()
{
	TArray<UWidget*> children = Grid->GetAllChildren();
	
	for (UWidget* child : children)
		Items.Add(child->GetName(),Cast<UCUserWidget_Item>(child));

	Inventory = CHelpers::GetComponent<UCInventoryComponent>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	Super::NativeConstruct();

}

void UCUserWidget_Inventory::ToggleInventory()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);

	bShow == false ? SetVisibility(ESlateVisibility::Visible) : SetVisibility(ESlateVisibility::Hidden);

	CheckNull(Inventory);

	bShow == false ? Inventory->SetOnInventory() : Inventory->SetOffInventory();

	if (bShow == false)
	{
		UpdateInventory();
	}
	
	bShow = !bShow;
}

void UCUserWidget_Inventory::Clicked(FString InName)
{
	TSubclassOf<ACObject_Potion> objectClass = Items[InName]->GetSlotContent().Item.ItemClass;                        
	CheckNull(objectClass);

	FTransform transform = FTransform();
	ACObject_Potion* potion = GetWorld()->SpawnActor<ACObject_Potion>(objectClass, transform);
	TArray<FSlotDesc>& inven = Inventory->GetInventory();
	FString name = potion->GetItemDesc().Name;
	for (FSlotDesc& desc : inven)
	{
		if (name == desc.Item.Name)
		{
			desc.Quantity = FMath::Clamp(desc.Quantity - 1, 0, 1);
			if (desc.Quantity == 0)
			{
				desc = FSlotDesc();
				Inventory->SortInventory();
			}
			break;
		}
	}
	potion->UseItem();
	UpdateInventory();
}

void UCUserWidget_Inventory::UpdateInventory()
{
	const TArray<FSlotDesc>& inven = Inventory->GetInventory();
	int32 index = 0;
	
	for (TPair<FString, UCUserWidget_Item*> it : Items)
	{
		it.Value->UpdateSlot(inven[index++]);
	}

	MoneyText->SetText(UKismetTextLibrary::Conv_IntToText(static_cast<int32>(Inventory->GetMoney()))); 
}
