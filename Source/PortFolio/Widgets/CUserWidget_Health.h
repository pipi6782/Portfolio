#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Health.generated.h"

UCLASS()
class PORTFOLIO_API UCUserWidget_Health : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void UpdateHealth(float InValue);
	
	void UpdateMaxHealth(float InValue);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* Grid;

	UPROPERTY(BlueprintReadOnly)
		TArray<class UCUserWidget_Heart*> Hearts;
};
