#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject_DropItem.h"
#include "Components/TimelineComponent.h"
#include "CObject_Life.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Life : public ACObject_DropItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* LifeFront;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* LifeBehind;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* LifeMain;

	UPROPERTY(EditAnywhere)
		float ActivationTime = 1.0f;

	UPROPERTY(EditAnywhere)
		float MaxHeight = 10.0f;

public:
	ACObject_Life();

	virtual void Tick(float DeltaSeconds) override;



protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnLifeSpawned(float Output);

	UFUNCTION()
		void FinishSpawned();

private:
	FTimeline Timeline;
	UCurveFloat* Curve;
};
