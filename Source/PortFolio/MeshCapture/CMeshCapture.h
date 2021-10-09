// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMeshCapture.generated.h"

UCLASS()
class PORTFOLIO_API ACMeshCapture : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class USceneCaptureComponent2D* SceneCapture;

public:	
	ACMeshCapture();
};
