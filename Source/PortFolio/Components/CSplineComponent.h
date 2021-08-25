// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSplineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCSplineComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACSplinePath* Path;

public:	
	UCSplineComponent();
	FORCEINLINE int32 GetIndex() { return index; }
	FORCEINLINE ACSplinePath* GetPath() { return Path; }

protected:
	virtual void BeginPlay() override;

public:
	bool GetMoveTo(FVector& OutLocation);
	void UpdateSplineRoute(const FVector& InLocation);

	void Reset();
	void UpdateNextIndex(const FVector& InLocation);

private:
	int32 index = 0;
};
