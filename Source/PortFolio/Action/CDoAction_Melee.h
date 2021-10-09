// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Melee.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()
	
public:
	void EnableCombo(); 
	void DisableCombo();

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class AActor* InOtherActor) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class AActor* InOtherActor) override;

private:
	TArray<class AActor*> DamagedActor;
	bool bEnable, bExist;
	int32 Index;
};
