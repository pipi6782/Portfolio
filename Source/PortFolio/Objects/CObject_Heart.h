// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject_DropItem.h"
#include "CObject_Heart.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACObject_Heart : public ACObject_DropItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		float RotationCycle = 1.0f;

public:
	ACObject_Heart();

	virtual void Tick(float DeltaSeconds) override;

	virtual void Interact(class AActor* InInteractor) override;

protected :
	virtual void BeginPlay() override;
};
