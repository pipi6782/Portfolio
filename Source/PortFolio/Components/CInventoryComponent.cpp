#include "CInventoryComponent.h"
#include "Global.h"

#include "Character/CPlayer.h"
#include "Widgets/CUserWidget_Inventory.h"

UCInventoryComponent::UCInventoryComponent()
{
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PrepareInventory();

	player = Cast<ACPlayer>(GetOwner());
}

void UCInventoryComponent::SetOnInventory()
{
	bUsingInventory = true;
}

void UCInventoryComponent::SetOffInventory()
{
	bUsingInventory = false;
}

void UCInventoryComponent::AddMoney(float InValue)
{
	Money += InValue;
}

void UCInventoryComponent::SortInventory()
{
	for (int i = 0; i < Inventory.Num()-1; i++)
	{
		//���� �ε����� ���� ���� �����ͼ� ������ 1�̻��̸� ���� �ε����� �̵�
		//0���� ���� �ε����� ��ü
		if (Inventory[i].Quantity > 0) continue;
		Inventory.Swap(i, i + 1);
	}
}

void UCInventoryComponent::ToggleInventory()
{
	player->GetInventoryWidget()->ToggleInventory();
}

bool UCInventoryComponent::AddToInventory(FSlotDesc InSlotDesc)
{
	if (InSlotDesc.Item.bStackable == false)
	{
		CreateStack(InSlotDesc);
		return true;
	}
	return true;
}

void UCInventoryComponent::PrepareInventory()
{
	Inventory.SetNum(SlotAmount);
	for (FSlotDesc desc : Inventory)
	{
		desc = FSlotDesc();
	}
}

void UCInventoryComponent::CreateStack(FSlotDesc InSlotDesc)
{
	FSlotDesc temp = FSlotDesc();
	int32 index = 0;
	while (Inventory[index].Item.Name.Compare("") != 0)
	{
		index++;
	}
	Inventory[index] = InSlotDesc;
}
