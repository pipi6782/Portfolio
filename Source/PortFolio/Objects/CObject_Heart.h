// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "CObject_Heart.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACObject_Heart : public ACObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		float RotationCycle = 1.0f;
public:
	ACObject_Heart();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected :
	virtual void BeginPlay() override;
};
