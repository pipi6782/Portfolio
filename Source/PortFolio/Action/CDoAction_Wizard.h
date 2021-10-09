#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Components/CActionComponent.h"
#include "CDoAction_Wizard.generated.h"


UCLASS()
class PORTFOLIO_API ACDoAction_Wizard : public ACDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle;
	
		class UParticleSystemComponent* ParticleComponent;
protected:
	virtual void BeginPlay() override;

public:
	ACDoAction_Wizard();

	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	void StartThrowing();

	void OffParticle();

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);

private:
	class UCActionComponent* Action;
};
