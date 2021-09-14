// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class PORTFOLIO_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		bool bLoop;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;

public:
	ACPatrolPath();
	virtual void OnConstruction(const FTransform& Transform) override;

	class USplineComponent* GetSpline() { return Spline; }

protected:
	virtual void BeginPlay() override;

};