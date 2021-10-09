#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCustomStructs.h"
#include "Input/Events.h"
#include "CUserWidget_Item.generated.h"

UCLASS()
class PORTFOLIO_API UCUserWidget_Item : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetItemAmount(int32 InValue);

	UFUNCTION(BlueprintCallable)
		void ClickButton();

	void UpdateSlot(const FSlotDesc& InDesc);

public:
	FORCEINLINE const FSlotDesc& GetSlotContent() { return SlotContent; }
	FORCEINLINE class UTextBlock* GetTextBlock() { return Text; }


private:
	class UCUserWidget_Inventory* GetInventoryWidget();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
		class UImage* BorderImage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
		class UImage* Image;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Text;

	UPROPERTY(BlueprintReadOnly)
		class UCInventoryComponent* Inventory;

	FSlotDesc SlotContent;
};
