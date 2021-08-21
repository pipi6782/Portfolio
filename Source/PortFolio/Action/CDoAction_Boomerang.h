// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Components/TimelineComponent.h"
#include "CDoAction_Boomerang.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACDoAction_Boomerang : public ACDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float timer = 0.001f;
public:
	ACDoAction_Boomerang();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;
	
private:
	UFUNCTION()
		void ReturnToPlayer();

	UFUNCTION()
		void OnBoomerangEnd();

	UFUNCTION()
		void OnBoomerangThrowing(float Output);

private:
	class ACAttachment* boomerang;
	class UCSplineComponent* Spline;

	FTimeline Timeline;

};
