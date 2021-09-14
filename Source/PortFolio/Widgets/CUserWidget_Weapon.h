// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UCUserWidget_Weapon : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
		class UBorder* WeaponBorder;

	UPROPERTY(meta = (BindWidget))
		class UButton* WeaponButton;

public:
	UFUNCTION(BlueprintCallable)
		void PressingButton();

	UFUNCTION(BlueprintCallable)
		void UnHoverButton();

	UFUNCTION(BlueprintCallable)
		void Clickedbutton();

};
