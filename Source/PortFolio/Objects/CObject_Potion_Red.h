#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject_Potion.h"
#include "CObject_Potion_Red.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Potion_Red : public ACObject_Potion
{
	GENERATED_BODY()
	
public:
	ACObject_Potion_Red();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
