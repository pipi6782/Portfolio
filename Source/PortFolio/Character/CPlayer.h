// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTFOLIO_API ACPlayer : public ACharacter
{
	GENERATED_BODY()


private:
	//SceneComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Pitching;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_Health> HealthWidgetClass;


private:
	//ActorComponent
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UCSplineComponent* Spline;

private:
	UPROPERTY(EditDefaultsOnly)
		float WalkDistance = 200.0f;

	UPROPERTY(EditDefaultsOnly)
		float RunDistance = 400.0f;

	UPROPERTY(EditDefaultsOnly)
		float SprintDistance = 600.0f;

	UPROPERTY(EditDefaultsOnly)
		float ThrowDistance = 300.0f;

public:
	FORCEINLINE class UCStatusComponent* GetStatusComponent() { return Status; }
	FORCEINLINE class UCUserWidget_Health* GetHealthWidget() { return HealthWidget; }
	FORCEINLINE void ResetHitResult() { HitResult = FHitResult();}

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnAction();
	void OffAction();

	void OnChangeWeapon();

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveToDestination();

	void SetDestination();

	void DrawObjectLine();

private:
	FHitResult HitResult;

	bool bClicked = false;

	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Health* HealthWidget;

	float ClickTime = 0.0f;

	bool bOverleppedObject = false;

	TArray<AActor*> OverlappedActors;
};
