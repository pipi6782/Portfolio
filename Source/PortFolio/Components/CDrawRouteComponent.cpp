#include "CDrawRouteComponent.h"
#include "Global.h"
#include "Action/CDrawLine.h"

UCDrawRouteComponent::UCDrawRouteComponent()
{
}


// Called when the game starts
void UCDrawRouteComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACDrawLine::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		Draw = Cast<ACDrawLine>(actor);
		if (!!Draw) break;
	}
}

void UCDrawRouteComponent::DrawLine()
{
	Draw->Draw();
}
