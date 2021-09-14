#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ICharacter.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "CEnemy.generated.h"

UCLASS()
class PORTFOLIO_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

protected: 
	//ActorCompnent
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Damaged();
	void Dead();

public:
	ACEnemy();

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void End_Dead() override;
protected:
	virtual void BeginPlay() override;

private:
	class AController* DamageInstigator;

};
