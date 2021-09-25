#include "CEnvQueryContext_StrafeLeft.h"
#include "Global.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

#include "Character/CPlayer.h"
#include "Character/CEnemy_Boss.h"
#include "Components/CBehaviorComponent.h"


void UCEnvQueryContext_StrafeLeft::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(QueryInstance.Owner.Get());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(boss->GetController());

	ACPlayer* player = behavior->GetTargetPlayer();

	if (!!player)
	{
		FVector bossLocation = boss->GetActorLocation();
		FVector playerLocation = player->GetActorLocation();
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(bossLocation, playerLocation);
		FVector left = FQuat(rotation).GetRightVector() * -300.0f;

		UEnvQueryItemType_Point::SetContextHelper(ContextData, bossLocation + left);
	}
	else
	{
		UEnvQueryItemType_Point::SetContextHelper(ContextData, FVector(0, 0, 0));
	}
}
