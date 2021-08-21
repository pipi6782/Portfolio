#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrawLine.generated.h"

UCLASS()
class PORTFOLIO_API ACDrawLine : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDrawLine();

protected:
	virtual void BeginPlay() override;

public:	
	void Draw();

private:
	//class UMaterialInstanceDaynamic
};
