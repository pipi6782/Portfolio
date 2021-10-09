#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		FLinearColor DrawColor;

	UPROPERTY(EditAnywhere)
		FLinearColor FlyColor;

public:	
	ACDrawLine();

	virtual void Tick(float DeltaSeconds) override;

	bool CheckLength(const FVector& InLocation);

	void SetFlying();

protected:
	virtual void BeginPlay() override;

public:
	void Draw();

	UFUNCTION()
		void ResetPoints();

private:
	TArray<FVector> Locations;
	bool bFly = false;
};
