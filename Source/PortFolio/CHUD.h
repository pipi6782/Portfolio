#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class PORTFOLIO_API ACHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		FLinearColor DrawColor;

	UPROPERTY(EditDefaultsOnly)
		FLinearColor FlyingColor;

public:
	ACHUD();

public:
	FORCEINLINE bool CanDraw() { return bDraw; }
	FORCEINLINE void EnableDraw() { bDraw = true; }
	FORCEINLINE void DisableDraw() { bDraw = false; }

	FORCEINLINE void SetOnFlying() { bFlying = true; }

	virtual void DrawHUD() override;

	void AddPoint(float InX, float InY);
	void ClearPoints();
private:
	bool bDraw;
	bool bFlying;

	TArray<FVector2D> Points;
};
