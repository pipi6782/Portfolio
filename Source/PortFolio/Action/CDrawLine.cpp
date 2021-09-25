#include "CDrawLine.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "CHUD.h"

ACDrawLine::ACDrawLine()
{
}

void ACDrawLine::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACDrawLine::BeginPlay()
{
	Super::BeginPlay();
	Hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<ACHUD>();
}

bool ACDrawLine::CheckLength(const FVector& InLocation)
{
	FVector location = Particles[Particles.Num() - 1]->GetComponentTransform().GetLocation();
	float distance = FVector::Dist2D(location, InLocation);
	if (distance >= 20.0f) return true;
	else return false;
}

void ACDrawLine::Draw()
{
	CheckNull(Hud);
	float locationX, locationY;
	bool b = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(locationX, locationY);
	if (b == true)
		Hud->AddPoint(locationX, locationY);
}



void ACDrawLine::ResetPoints()
{
	CheckNull(Hud);
	Hud->ClearPoints();
	Hud->DisableDraw();
}
