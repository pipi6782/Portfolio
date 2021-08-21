// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float RunSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float SprintSpeed = 600.0f;


public:
	//Health
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

public:
	//Speed
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetRunSpeed() { return RunSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }

	FORCEINLINE bool CanMove() { return bCanMove; }

	UFUNCTION(BlueprintCallable)	void SetMove();
	UFUNCTION(BlueprintCallable)	void SetStop();

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void AddMaxHealth();

public:	
	// Sets default values for this component's properties
	UCStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health;
	bool bCanMove = false;
	class ACPlayer* OwnerCharacter;
};
