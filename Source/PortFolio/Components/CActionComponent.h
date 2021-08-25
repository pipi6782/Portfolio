// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8 //어떤 무기를 장착하고있는가
{
	Unarmed,	//칼을 획득하지 않았을때
	Sword,		//검을 획득한 이후 쭉(maybe)
	Throwing,	//던질수 있는 오브젝트를 들고 있을 때
	Boomerang,	//부메랑 사용중 취하는 액션
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DataAssets[(int32)EActionType::Max];

public:	
	UCActionComponent();

public:
	FORCEINLINE class UCAction* GetCurrent() { return Datas[static_cast<int32>(Type)]; }


	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSwordMode() { return Type == EActionType::Sword; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsThrowingMode() { return Type == EActionType::Throwing; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBoomerangMode() { return Type == EActionType::Boomerang; }

	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();
	UFUNCTION(BlueprintCallable)
		void SetSwordMode();
	UFUNCTION(BlueprintCallable)
		void SetThrowingMode();
	UFUNCTION(BlueprintCallable)
		void SetBoomerangMode();

public:
	FORCEINLINE int32 GetType() { return static_cast<int32>(Type); }

public:
	void DoAction();

private:
	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:	
	//Purpose : 무기에 따른 블렌드 포즈 변경
	//Call : Set(EActionType)Mode 호출시
	//Bind : CAnimInstance->OnTypeChanged 
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType	Type;

	UPROPERTY()
		class UCAction* Datas[static_cast<int32>(EActionType::Max)];

};
