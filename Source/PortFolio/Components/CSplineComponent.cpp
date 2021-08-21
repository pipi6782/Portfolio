#include "CSplineComponent.h"
#include "Global.h"
#include "Components/SplineComponent.h"

#include "Action/CSplinePath.h"

UCSplineComponent::UCSplineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCSplineComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACSplinePath::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		Path = Cast<ACSplinePath>(actor);
		if (!!Path) break;
	}

}

//������ ��ġ�� ���ϴ� �Լ�
bool UCSplineComponent::GetMoveTo(FVector& OutLocation)
{
	OutLocation = FVector::ZeroVector;
	CheckNullResult(Path, false);
	CheckTrueResult((index == Path->GetSpline()->GetNumberOfSplinePoints() - 1), false);
	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World);
	return true;
}

//���ö��� ����Ʈ�� �߰��Ѵ�. 
void UCSplineComponent::UpdateSplineRoute(const FVector& InLocation)
{
	Path->UpdateSplinePath(InLocation);
}

void UCSplineComponent::Reset()
{
	index = 0;
	Path->ResetSpline();
}

void UCSplineComponent::UpdateNextIndex(const FVector& InLocation)
{
	CheckFalse(Path->GetSpline()->GetNumberOfSplinePoints() > 0);
	FVector location = Path->GetSpline()->GetLocationAtSplinePoint(index,ESplineCoordinateSpace::World);
	if (FMath::IsNearlyZero(FVector::Dist2D(location, InLocation), 10.0f))
	{
		index++;
		index = FMath::Clamp(index, 0, Path->GetSpline()->GetNumberOfSplinePoints()-1);
	}
}