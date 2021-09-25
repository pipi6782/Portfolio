// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		float StrafeRange = 600.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }
	FORCEINLINE float GetStrafeRange() { return StrafeRange; }
	FORCEINLINE bool CanAttack() { return bAttack; }


	void EnableAttack();
	void DisableAttack();
public:
	ACAIController();

	virtual void Tick(float DeltaTime) override;
public:
	float GetSightRadius();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	bool bAttack = false;

	class ACEnemy_AI* OwnerEnemy;
	class ACEnemy_Boss* OwnerBoss;

	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;
};
