#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Particle;

public:	
	ACDrawLine();

	virtual void Tick(float DeltaSeconds) override;

	bool CheckLength(const FVector& InLocation);
protected:
	virtual void BeginPlay() override;

public:
	void Draw(const FVector& InLocation);

	UFUNCTION()
		void ResetParticles();

private:
	TArray<class UParticleSystemComponent*> Particles;
};
