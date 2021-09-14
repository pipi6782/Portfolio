// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CBTService_Strafe.generated.h"

UCLASS()
class PORTFOLIO_API UCBTService_Strafe : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Strafe();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void GetStrafeQuery(class ACAIController* InBossController, class UEnvQuery*& OutQuery);

	void MoveToStrafePoint(TSharedPtr<FEnvQueryResult> result);
};
