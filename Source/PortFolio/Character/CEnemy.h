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

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	
protected:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	void Damaged();
	void Dead();

public:
	ACEnemy();

	virtual void End_Dead() override;

	void DestroyMagicBallAndStopMontage();

	void FollowBoomerang(AActor* InAttachment);
	void UnfollowBoomerang();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

protected:
	class AController* DamageInstigator;

	AActor* followAttachment;

	bool bFollow = false;
};
