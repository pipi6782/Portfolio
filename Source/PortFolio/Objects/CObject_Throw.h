#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "CObject_Throw.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectThrown);

UCLASS()
class PORTFOLIO_API ACObject_Throw : public ACObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UDestructibleComponent* Destructible;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

public:
	ACObject_Throw();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

	virtual void Begin_Interact(class ACharacter* InCharacter);

	UFUNCTION()
		void OnThrown();

public:
	FObjectThrown OnObjectThrown;
};
