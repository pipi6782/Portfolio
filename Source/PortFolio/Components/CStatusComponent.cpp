#include "CStatusComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Widgets/CUserWidget_Health.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

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
	//TODO : ü�� ������ ���� ���� ����
	CheckNull(OwnerCharacter);
	OwnerCharacter->GetHealthWidget()->UpdateHealth(Health);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	OwnerCharacter->GetHealthWidget();
	//TODO : ü�� ���ҿ� ���� ���� ����
	CheckNull(OwnerCharacter);
	OwnerCharacter->GetHealthWidget()->UpdateHealth(Health);
}

void UCStatusComponent::AddMaxHealth()
{
	MaxHealth += 1;;
	//TODO : �ִ�ü�� ������ ���� ���� ����
	CheckNull(OwnerCharacter);

	OwnerCharacter->GetHealthWidget()->UpdateMaxHealth(MaxHealth);
}
