#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject_DropItem.h"
#include "CObject_Potion.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Potion : public ACObject_DropItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
		float PotionValue = 0.0f;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Particle;

	UPROPERTY(EditDefaultsOnly)
		FItemDesc ItemDesc;

public:
	virtual void Interact(AActor* InInteractor) override;
	virtual void UseItem() override;
	FORCEINLINE const FItemDesc& GetItemDesc() { return ItemDesc; }
};
