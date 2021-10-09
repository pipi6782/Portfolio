#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Inventory.generated.h"

UCLASS()
class PORTFOLIO_API UCUserWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
		void ToggleInventory();

	void Clicked(FString InName);

	void UpdateInventory();
protected:
	class UCInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* Grid;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ButtonClose;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* MoneyText;

	TMap<FString,class UCUserWidget_Item*> Items;

	bool bShow = false;
};
