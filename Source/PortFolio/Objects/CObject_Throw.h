#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "CObject_Throw.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectThrown);

UCLASS()
class PORTFOLIO_API ACObject_Throw : public ACObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UDestructibleComponent* Destructible;

	//캐릭터와 오브젝트가 상호작용할 수 있는지 여부를 판단
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

public:
	ACObject_Throw();

protected:
	virtual void BeginPlay() override;

	virtual void Begin_Interact(class ACharacter* InCharacter);

	UFUNCTION()
		void OnThrown();

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FObjectThrown OnObjectThrown;
};
