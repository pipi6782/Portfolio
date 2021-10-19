// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UCBTService_Boss : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_Boss();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
<<<<<<< HEAD

private:
	UFUNCTION()
		void StartSpawn();

private:
	bool bStartedSpawn = false;
	bool bCanSpawn = false;

=======
>>>>>>> parent of 8268443 (UE_2021_10_18_Final)
};
