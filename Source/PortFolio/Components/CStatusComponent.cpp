#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/CPlayer.h"
#include "Widgets/CUserWidget_Health.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = 0.5f;

	OwnerCharacter = Cast<ACPlayer>(GetOwner());

	CheckNull(OwnerCharacter);
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	OwnerCharacter->GetHealthWidget();
	
	CheckNull(OwnerCharacter);
	OwnerCharacter->GetHealthWidget()->UpdateHealth(Health);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	OwnerCharacter->GetHealthWidget();
	
	CheckNull(OwnerCharacter);
	OwnerCharacter->GetHealthWidget()->UpdateHealth(Health);
}

void UCStatusComponent::AddMaxHealth()
{
	MaxHealth += 1;
	MaxHealth = FMath::Clamp(MaxHealth, 0.0f, 6.0f);

	CheckNull(OwnerCharacter);

	float remain = MaxHealth - Health;
	AddHealth(remain);
	OwnerCharacter->GetHealthWidget()->UpdateMaxHealth(MaxHealth);
}

void UCStatusComponent::SetSpeed(ECharacterSpeed InSpeed)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());
	movement->MaxWalkSpeed = Speed[(int32)InSpeed];
}
