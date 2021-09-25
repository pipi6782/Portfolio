#pragma once

#include "CoreMinimal.h"
#include "Objects/CObject.h"
#include "CObject_Money.generated.h"

UCLASS()
class PORTFOLIO_API ACObject_Money : public ACObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

public:
	ACObject_Money();

protected:
	virtual void BeginPlay() override;


};
