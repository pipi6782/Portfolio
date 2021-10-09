// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Interfaces/ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Action/CActionData.h"
#include "Components/CMontageComponent.h"
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

	UPROPERTY(EditDefaultsOnly)
		class UCInventoryComponent* Inventory;

private:
	UPROPERTY(EditDefaultsOnly)
		float SprintDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly)
		float ThrowDistance = 300.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Health* HealthWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Weapon* WeaponWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UCUserWidget_Inventory* InventoryWidget;

public:
	FORCEINLINE class UCStatusComponent* GetStatusComponent() { return Status; }
	FORCEINLINE class UCUserWidget_Health* GetHealthWidget() { return HealthWidget; }
	FORCEINLINE class UCUserWidget_Inventory* GetInventoryWidget() { return InventoryWidget; }
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

	void Damaged();

	void Dead();
	virtual void End_Dead() override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void EnableHidden();
	void DisableHidden();

	void PlayMontage(const FMontageData* InData);
	void PlayMontage(const FDoActionData* InData);

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	void OnAction();
	void OffAction();

	void OnInteract();

	void OnInventory();

	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	

	//void MoveToDestination();

	//void SetDestination();

	void TraceObject(TArray<FHitResult>& OutHits);

	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
private:
	FHitResult HitResult;


	bool bClicked;

	bool bHideInZone = false;

	class UMaterialInstanceDynamic* OutlineMaterial;
	class AController* DamageInstigator;

	TSubclassOf<class UCUserWidget_Health> HealthWidgetClass;
	TSubclassOf<class UCUserWidget_Weapon> WeaponWidgetClass;
	TSubclassOf<class UCUserWidget_Inventory> InventoryWidgetClass;

};
