#include "CUserWidget_Weapon.h"
#include "Global.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "GameFramework/Character.h"

#include "Components/CActionComponent.h"
#include "Components/CInventoryComponent.h"

void UCUserWidget_Weapon::PressingButton()
{
	CheckNull(WeaponBorder);

	WeaponBorder->SetBrushColor(FLinearColor::Yellow);
}

void UCUserWidget_Weapon::UnHoverButton()
{
	CheckNull(WeaponBorder);

	WeaponBorder->SetBrushColor(FLinearColor::Black);
}

void UCUserWidget_Weapon::Clickedbutton()
{
	CheckNull(WeaponButton);

	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();

	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(character);

	CheckNull(inventory);
	CheckTrue(inventory->IsUsingInventory());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);

	CheckNull(action);

	action->SetBoomerangMode();
}
