#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "CCustomStructs.h"
#include "Interfaces/IInteractItem.h"
#include "CObject_DropItem.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_DropItem : public ACObject, public IIInteractItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

public:
	ACObject_DropItem();

	UFUNCTION()
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	virtual void Interact(class AActor* InInteractor) override {}

	void SetDrop(bool InOnGround = true);
protected:
	bool bOnGround = false;

};
