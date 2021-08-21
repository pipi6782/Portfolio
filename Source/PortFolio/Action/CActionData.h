// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CAction.h"
#include "CActionData.generated.h"


UCLASS()
class PORTFOLIO_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);

private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<TSubclassOf<class ACAttachment>> AttachmentClasses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;


};
