// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Interfaces/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTFOLIO_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Team")
		uint8 TeamID = 0;

private:
	//SceneComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* OutlineMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Pitching;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_Health> HealthWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCUserWidget_Weapon> WeaponWidgetClass;

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

	UPROPERTY(VisibleDefaultsOnly)
		class UCDrawRouteComponent* Draw;

private:
	UPROPERTY(EditDefaultsOnly)
		float SprintDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly)
		float ThrowDistance = 300.0f;

public:
	FORCEINLINE class UCStatusComponent* GetStatusComponent() { return Status; }
	FORCEINLINE class UCUserWidget_Health* GetHealthWidget() { return HealthWidget; }
	FORCEINLINE void ResetHitResult() { HitResult = FHitResult();}
	FORCEINLINE bool IsHiding() { return bHideInZone; }

public:
	ACPlayer();

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	void Dead();
	virtual void End_Dead() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void EnableHidden();
	void DisableHidden();

private:
	void OnAction();
	void OffAction();
	
	void MoveToDestination();

	void SetDestination();

	void TraceObject();

private:
	FHitResult HitResult;
	TArray<FHitResult> OutHits;

	bool bClicked = false;

	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Health* HealthWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Weapon* WeaponWidget;

	bool bHideInZone = false;

	float ClickTime = 0.0f;

	class UMaterialInstanceDynamic* OutlineMaterial;
	class AController* DamageInstigator;
};
