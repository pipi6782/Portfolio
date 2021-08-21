#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class PORTFOLIO_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }

public:	
	ACEquipment();

public:
	//PlayEquipMontage, ChangeBodyMaterial
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//Socket? Particle? -> Delegate Excute
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//State -> Idle
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	//OrientRotation -> true
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

protected:
	virtual void BeginPlay() override;

public:
	//Purpose : Set(EActionType)Mode 호출 시 Attachment(무기)의 소켓을 변경
	//Call : Begin_Equip이 노티파이에서 호출될때
	//Bind : Attachment::OnEquip ->AttachToComponent
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	FEquipmentData Data;
	FLinearColor Color;

};
