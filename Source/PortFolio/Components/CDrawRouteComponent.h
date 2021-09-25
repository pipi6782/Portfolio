#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDrawRouteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCDrawRouteComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	class ACDrawLine* Draw;

public:	
	UCDrawRouteComponent();

protected:
	virtual void BeginPlay() override;

public:
	void DrawLine();
};
