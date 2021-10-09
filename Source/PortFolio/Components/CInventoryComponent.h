#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCustomStructs.h"
#include "CInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		FString InventoryName;

	UPROPERTY(EditDefaultsOnly)
		int32 SlotAmount;

	UPROPERTY(EditDefaultsOnly)
		TArray<FSlotDesc> Inventory;

	UPROPERTY(VisibleDefaultsOnly, meta = (BindWidget))
		class UImage* Image;

public:
	FORCEINLINE int32 GetSlotAmount() { return SlotAmount; }
	FORCEINLINE TArray<FSlotDesc>& GetInventory() { return Inventory; }
	FORCEINLINE bool IsUsingInventory() { return bUsingInventory; }
	FORCEINLINE float GetMoney() { return Money; }
	void SetOnInventory();
	void SetOffInventory();
	void AddMoney(float InValue);
	void SortInventory();

public:
	void ToggleInventory();

	bool AddToInventory(FSlotDesc InSlotDesc);

	void PrepareInventory();

	void CreateStack(FSlotDesc InSlotDesc);

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

private:
	bool bUsingInventory;

	float Money = 0.0f;
	class ACPlayer* player;
};