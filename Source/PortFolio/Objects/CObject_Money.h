#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject_DropItem.h"
#include "Components/TimelineComponent.h"
#include "CObject_Money.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Money : public ACObject_DropItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		float MaxHeight = 10.0f;

	UPROPERTY(EditAnywhere)
		float ActivationTime = 1.0f;

public:
	ACObject_Money();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
private:
	void SetMoneyValue();

public:
	virtual void Interact(class AActor* InInteractor) override;

private:
	UFUNCTION()
		void OnSpawned(float Output);

	UFUNCTION()
		void FinishSpawned();

private:
	float Value = 0.0f;
	
	FTimeline Timeline;
	UCurveFloat* Curve;
};
