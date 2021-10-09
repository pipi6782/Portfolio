#include "CUserWidget_Item.h"
#include "Global.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetTextLibrary.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Character/CPlayer.h"
#include "CUserWidget_Inventory.h"

void UCUserWidget_Item::NativeConstruct()
{
	CheckNull(SlotContent.Item.Thumbnail);
	Image->SetBrushFromMaterial(SlotContent.Item.Thumbnail);
	UpdateSlot(SlotContent);
	SetItemAmount(SlotContent.Quantity);

	Super::NativeConstruct();
}

void UCUserWidget_Item::SetItemAmount(int32 InValue)
{
	if (InValue == 0)
	{
		Text->SetVisibility(ESlateVisibility::Hidden);
		BorderImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	BorderImage->SetVisibility(ESlateVisibility::Visible);
	Text->SetVisibility(ESlateVisibility::Visible);
	Text->SetText(UKismetTextLibrary::Conv_IntToText(InValue));
}

void UCUserWidget_Item::ClickButton()
{
	GetInventoryWidget()->Clicked(GetName());
}

void UCUserWidget_Item::UpdateSlot(const FSlotDesc& InDesc)
{
	SlotContent = InDesc;
	Image->SetBrushFromMaterial(SlotContent.Item.Thumbnail);
	SetItemAmount(SlotContent.Quantity);
	if (SlotContent.Quantity == 0)
	{
		BorderImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

UCUserWidget_Inventory* UCUserWidget_Item::GetInventoryWidget()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	return player->GetInventoryWidget();
}
