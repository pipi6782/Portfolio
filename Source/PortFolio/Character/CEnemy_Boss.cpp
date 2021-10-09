#include "CEnemy_Boss.h"
#include "Global.h"

#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	CHelpers::GetClass(&AIControllerClass, "Blueprint'/Game/Enemies/Boss/BP_CAIController_Boss.BP_CAIController_Boss_C'");
}

float ACEnemy_Boss::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;

	if (GetActorLabel().Contains("Final") == false)
	{
		return 0.0f;
	}

	if (DamageCauser->GetName().Contains("Boomerang"))
	{
		Status->SubHealth(0.0f);
		return 0.0f;
	}

	Status->SubHealth(damage);
	
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetDamagedMode();

	return Status->GetHealth();
}