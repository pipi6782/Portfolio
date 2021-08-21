// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Throw.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ACDoAction_Throw : public ACDoAction
{
	GENERATED_BODY()
	
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
};
