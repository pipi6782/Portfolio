#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObjectInteract, class ACharacter*, InCharacter);

UCLASS()
class PORTFOLIO_API ACObject : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* Scene;

public:	
	ACObject();

	virtual void Tick(float DeltaSeconds) override;
protected:
	UFUNCTION()
		virtual void Begin_Interact(class ACharacter* InCharacter) {}
protected:
	virtual void BeginPlay() override;

public:
	FObjectInteract OnObjectInteract;

protected:
	class ACharacter* InteractedCharacter;
};
