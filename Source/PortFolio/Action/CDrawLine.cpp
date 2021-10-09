#include "CDrawLine.h"
#include "Global.h"
#include "Components/SplineComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "CHUD.h"

ACDrawLine::ACDrawLine()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACDrawLine::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CheckTrue(Locations.Num() <= 1);
	for (int32 i = 1; i < Locations.Num(); i++)
	{
		FVector start = Locations[i - 1];
		FVector end = Locations[i];
		GetWorld()->LineBatcher->DrawLine(start, end, bFly ? FlyColor : DrawColor, 0.0f, 5.0f, 0);
	}
}

void ACDrawLine::BeginPlay()
{
	Super::BeginPlay();
}

bool ACDrawLine::CheckLength(const FVector& InLocation)
{
	CheckTrueResult((Locations.Num() == 0), true);
	FVector last = Locations[Locations.Num() - 1];
	float distance = FVector::Distance(last, InLocation);
	if (distance <= 10.0f) return false;
	else return true;
}

void ACDrawLine::SetFlying()
{
	bFly = true;
}

void ACDrawLine::Draw()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(controller);
	APawn* pawn = controller->GetPawn();

	FHitResult hitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> quries;
	quries.Add(EObjectTypeQuery::ObjectTypeQuery1);

	controller->GetHitResultUnderCursorForObjects(quries, true, hitResult);

	FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, pawn->GetActorLocation().Z - 20.0f);

	CheckFalse(CheckLength(location));
	
	Locations.Add(location);
}

void ACDrawLine::ResetPoints()
{
	Locations.Empty();
	bFly = false;
}
