// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDrawRouteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCDrawRouteComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	class ACDrawLine* Draw;

public:	
	// Sets default values for this component's properties
	UCDrawRouteComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void DrawLine(const FVector& InLocation);
};
