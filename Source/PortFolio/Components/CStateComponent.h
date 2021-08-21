#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8 //���� ĳ������ ���¸� ��Ÿ��
{
	Idle, Action, Equip, Rolling, Interacting, Damaged, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()
		
private:
	UPROPERTY(EditAnywhere)
		float Test1 = 100.0f;
	UPROPERTY(EditAnywhere)
		FString Test2;

public:
	FORCEINLINE EStateType GetType() { return Type; }
public:	
	UCStateComponent();

public:
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsRollingMode() { return Type == EStateType::Rolling; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsDamagedMode() { return Type == EStateType::Damaged; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsInteractingMode() { return Type == EStateType::Interacting; }

public :
	void SetIdleMode();
	void SetActionMode();
	void SetEquipMode();
	void SetRollingMode();
	void SetDamagedMode();
	void SetInteractingMode();

private:
	void ChangeType(EStateType InNewType);

public:
	//Purpose : ĳ���� ���� ���濡 ���� ���� �̺�Ʈ �߻�
	//Call : Set(EStateType)Mode ȣ���
	//Bind : MontageComponent->Play�ۡۡ�Montage
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;

protected:
	virtual void BeginPlay() override;

private:
	EStateType Type;
};
