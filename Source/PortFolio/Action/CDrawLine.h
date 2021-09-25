#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDrawLine();

	virtual void Tick(float DeltaSeconds) override;

	bool CheckLength(const FVector& InLocation);
protected:
	virtual void BeginPlay() override;

public:
	void Draw();

	UFUNCTION()
		void ResetPoints();

private:
	TArray<class UParticleSystemComponent*> Particles;

	class ACHUD* Hud;
};
