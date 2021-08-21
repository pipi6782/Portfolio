// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSplinePath.generated.h"

UCLASS()
class PORTFOLIO_API ACSplinePath : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* Scene;
	
	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UTextRenderComponent* Text;

	UPROPERTY(EditAnywhere)
		float UpdateDistance = 10.0f;


	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

public:	
	// Sets default values for this actor's properties
	ACSplinePath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateSplinePath(const FVector& InLocation);
	void ResetSpline(); 
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }
	FORCEINLINE class UCurveFloat* GetCurve() { return Curve; }

};
