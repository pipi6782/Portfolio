// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_StrafeLeft.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UCEnvQueryContext_StrafeLeft : public UEnvQueryContext
{
	GENERATED_BODY()

private:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
