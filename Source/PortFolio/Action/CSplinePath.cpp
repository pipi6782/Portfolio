#include "CSplinePath.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACSplinePath::ACSplinePath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &Spline, "Spline", Scene);
	CHelpers::CreateComponent(this, &Text, "Text", Scene);

	Spline->SetRelativeLocation(FVector(0, 0, 30));

	Text->SetRelativeLocation(FVector(0, 0, 100));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;

	CHelpers::GetAsset(&Curve, "CurveFloat'/Game/Action/Curve_Boomerang.Curve_Boomerang'");
}

// Called when the game starts or when spawned
void ACSplinePath::BeginPlay()
{
	Super::BeginPlay();
	Text->SetVisibility(false);
	Spline->ClearSplinePoints();
}

// Called every frame
void ACSplinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACSplinePath::UpdateSplinePath(const FVector& InLocation)
{
	CheckFalse(Spline->GetSplineLength() < 30000);
	if (Spline->GetNumberOfSplinePoints() > 0)
	{
		FVector last = Spline->GetWorldLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1);
		if (FVector::Dist2D(last, InLocation) < UpdateDistance)
			return;
		Spline->AddSplineWorldPoint(InLocation);
	}
	else
	{
		Spline->AddSplineWorldPoint(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)->GetActorLocation());
		return;
	}
}

void ACSplinePath::ResetSpline()
{
	Spline->ClearSplinePoints();
}

