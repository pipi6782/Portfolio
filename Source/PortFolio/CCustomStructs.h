#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CCustomStructs.generated.h"

USTRUCT(BlueprintType)
struct FItemDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FString Name;

	UPROPERTY(EditAnywhere)
		bool bStackable;

	UPROPERTY(EditAnywhere)
		class UMaterialInstanceConstant* Thumbnail;

	UPROPERTY(EditAnywhere)
		FString Description;

	UPROPERTY(EditAnywhere)
		bool bConsumable;

	UPROPERTY(EditAnywhere)
		int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
		float Durability;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACObject_DropItem> ItemClass;

	FItemDesc()
		: Name("")
		, bStackable(false)
		, Thumbnail(nullptr)
		, Description("")
		, bConsumable(false)
		, MaxStackSize(0)
		, Durability(0.0f)
		, ItemClass(nullptr)
	{}

};

USTRUCT(BlueprintType)
struct FSlotDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItemDesc Item;

	UPROPERTY(EditAnywhere)
		int32 Quantity;

	FSlotDesc()
		: Item(FItemDesc())
		, Quantity(0)
	{}

};

USTRUCT(BlueprintType)
struct FRewardData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString RewardName;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACObject> ObjectClass;

	UPROPERTY(EditAnywhere)
		float Percentage;
};


UCLASS()
class PORTFOLIO_API ACCustomStructs : public AActor
{
	GENERATED_BODY()
	
};

