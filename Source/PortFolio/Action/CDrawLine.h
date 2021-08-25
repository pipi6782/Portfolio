#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USplineComponent* Spline;

public:	
	ACDrawLine();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void Draw(const FVector& InLocation);
};
