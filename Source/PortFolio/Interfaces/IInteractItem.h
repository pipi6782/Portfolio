// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractItem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTFOLIO_API IIInteractItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(class AActor* InInteractor) = 0;
	virtual void UseItem() {}
};

