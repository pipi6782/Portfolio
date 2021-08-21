// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CStateComponent.h"
#include "CMontageComponent.generated.h"



USTRUCT(BlueprintType)
struct FMontageData :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EStateType Type;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

public:
	void PlayDead();
	void PlayDamaged();

public:	
	// Sets default values for this component's properties
	UCMontageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InStateType);

private:
	FMontageData* Datas[static_cast<int32>(EStateType::Max)];
};
