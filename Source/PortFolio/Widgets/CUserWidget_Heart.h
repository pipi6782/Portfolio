// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Heart.generated.h"

UENUM(BlueprintType)
enum class EVisibleType : uint8
{
	Visible, None, Max
};

UCLASS()
class PORTFOLIO_API UCUserWidget_Heart : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FORCEINLINE void SetLeftVisible() { SetVisible(0,EVisibleType::Visible); }
	FORCEINLINE void SetLeftHidden() { SetVisible(0,EVisibleType::None);}

	FORCEINLINE void SetRightVisible() { SetVisible(1,EVisibleType::Visible); }
	FORCEINLINE void SetRightHidden() { SetVisible(1,EVisibleType::None); }

private:
	void SetVisible(int32 index, EVisibleType Type);


protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* Grid;

	UPROPERTY(BlueprintReadOnly)
		TArray<class UImage*> Hearts;
};
