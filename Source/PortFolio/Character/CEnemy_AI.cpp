#include "CEnemy_AI.h"
#include "Global.h"
#include "Components/CPatrolComponent.h"

#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"
ACEnemy_AI::ACEnemy_AI()
{
	CHelpers::CreateActorComponent<UCPatrolComponent>(this, &Patrol, "Patrol");
	CHelpers::GetClass(&AIControllerClass, "Blueprint'/Game/Enemies/AI/BP_CAIController.BP_CAIController_C'");
}

float ACEnemy_AI::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;

	//대미지를 준 대상이 부메랑인지 체크
	if (DamageCauser->GetName().Contains("Boomerang") || DamageCauser->GetName().Contains("Throw"))
	{
		Status->SubHealth(0.0f);
	}
	else
	{
		//캐릭터가 적의 전방을 공격했는지 체크
		float dotResult = GetDotProductTo(DamageInstigator->GetPawn());
		if (dotResult >= 0.0f)
		{
			Status->SubHealth(0.0f);
		}
		else
		{
			Status->SubHealth(damage);
		}
	}

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetDamagedMode();
	return Status->GetHealth();
}
