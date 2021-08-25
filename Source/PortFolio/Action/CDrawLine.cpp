#include "CDrawLine.h"
#include "Global.h"
#include "Components/SplineComponent.h"

ACDrawLine::ACDrawLine()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &Spline, "Spline",Scene);

	Scene->SetMobility(EComponentMobility::Static);
}

void ACDrawLine::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CLog::Print(Spline->GetNumberOfSplinePoints(), 1);
}

// Called when the game starts or when spawned
void ACDrawLine::BeginPlay()
{
	Super::BeginPlay();
	Spline->ClearSplinePoints();
}