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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	virtual void Begin_Interact(class ACharacter* InCharacter);

	UFUNCTION()
		void OnThrown();

	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void DetachActor();
	
	void SpawnObject(FVector InLocation);
public:
	//Purpose : 오브젝트를 던질때 실행할 함수들을 바인딩
	// Call : ACDoAction_Throw::Begin_DoAction 호출시
	//Bind : DetachActor, OnThrown
	FObjectThrown OnObjectThrown;

private:
	bool bDamaged = false;
};
