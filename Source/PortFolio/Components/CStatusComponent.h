#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200, 400, 600 };

public:
	//Health
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

public:
	//Speed
	FORCEINLINE float GetWalkSpeed() { return Speed[static_cast<int32>(ECharacterSpeed::Walk)]; }
	FORCEINLINE float GetRunSpeed() { return Speed[static_cast<int32>(ECharacterSpeed::Run)]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[static_cast<int32>(ECharacterSpeed::Sprint)]; }

	FORCEINLINE bool CanMove() { return bCanMove; }

	UFUNCTION(BlueprintCallable)	void SetMove();
	UFUNCTION(BlueprintCallable)	void SetStop();

	void AddHealth(float InAmount);
	void SubHealth(float InAmount);

	void AddMaxHealth();

	void SetSpeed(ECharacterSpeed InSpeed);

public:	
	// Sets default values for this component's properties
	UCStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health;
	bool bCanMove = true;
	class ACPlayer* OwnerCharacter;
};
