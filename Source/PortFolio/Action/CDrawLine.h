// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDrawLine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Draw();

private:
	UTextureRenderTarget2D RenderTarget;

};
