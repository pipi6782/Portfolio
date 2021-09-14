#include "CDrawLine.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACDrawLine::ACDrawLine()
{
}

void ACDrawLine::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool ACDrawLine::CheckLength(const FVector& InLocation)
{
	CheckTrueResult(Particles.Num() == 0, true);

	FVector location = Particles[Particles.Num() - 1]->GetComponentTransform().GetLocation();
	float distance = FVector::Dist2D(location, InLocation);
	if (distance >= 20.0f) return true;
	else return false;
}

// Called when the game starts or when spawned
void ACDrawLine::BeginPlay()
{
	Super::BeginPlay();
}

void ACDrawLine::Draw(const FVector& InLocation)
{
	CheckFalse(CheckLength(InLocation));
	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(FVector(1.5f, 1.5f, 1.5f));
	UParticleSystemComponent* particle = nullptr;
	if (!!Particle)
		particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, transform, false);

	Particles.Add(particle);
}



void ACDrawLine::ResetParticles()
{
	for (UParticleSystemComponent* particle : Particles)
	{
		particle->DestroyComponent();
	}

	Particles.Empty();
}
