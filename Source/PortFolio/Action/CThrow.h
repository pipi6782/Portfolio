#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBeginOverlap, FHitResult, InHitResult);

UCLASS()
class PORTFOLIO_API ACThrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly)
		FTransform HitEffectTransform;

	UPROPERTY(EditDefaultsOnly)
		FVector DesiredScale;

	UPROPERTY(EditDefaultsOnly)
		float DesiredTime;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* ThrowEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;


public:	
	ACThrow();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartThrowing();
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;

private:
	bool bThrown = false;

	float MoveSpeed = 0.0f;

};