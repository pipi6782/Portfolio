#include "CHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"

ACHUD::ACHUD()
{
	
}

void ACHUD::DrawHUD()
{
	CheckFalse(bDraw);
	CheckNull(Canvas);

	Super::DrawHUD();

	CheckFalse(Points.Num() >= 2);

	for (int32 i = 1; i < Points.Num(); i++)
	{
		FVector2D prev = Points[i - 1];
		FVector2D current = Points[i];
		FCanvasLineItem lineItem(prev, current);
		lineItem.SetColor(bFlying ? FlyingColor : DrawColor);
		lineItem.LineThickness = 10.0f;
		lineItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
		Canvas->DrawItem(lineItem);
	}
}

void ACHUD::AddPoint(float InX, float InY)
{
	FVector2D vec(InX, InY);
	if (Points.Num() > 0)
	{
		FVector2D last = Points[Points.Num()-1];
		CheckTrue(FMath::IsNearlyZero(FVector2D::Distance(vec,last),10.0f));
	}

	Points.Add(vec);
}

void ACHUD::ClearPoints()
{
	bFlying = false;
	Points.Empty();
}

