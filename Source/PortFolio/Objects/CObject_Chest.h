#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "Components/TimelineComponent.h"
#include "CCustomStructs.h"
#include "CObject_Chest.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Chest : public ACObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Top;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Bottom;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
		float OpeningSpeed = 100.0f;

public:
	ACObject_Chest();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Begin_Interact(class ACharacter* InCharacter) override;

	UFUNCTION()
		void Opening(float Output);

	UFUNCTION()
		void EndOpening();

	void SpawnObject(FVector InLocation);

	void GetRewardClass(TSubclassOf<ACObject_DropItem>* InClass);

	void SortRewardDatas();

private:
	FTimeline Timeline;
	UCurveFloat* Curve;

	TArray<FRewardData*> RewardDatas;
	UDataTable* Table;
};
