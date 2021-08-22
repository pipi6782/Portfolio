#include "CDoAction_Boomerang.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/SplineComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CSplineComponent.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CSplinePath.h"

ACDoAction_Boomerang::ACDoAction_Boomerang()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACDoAction_Boomerang::BeginPlay()
{
	Super::BeginPlay();

	//Get Boomerang from player
	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetName().Contains("Boomerang"))
		{
			boomerang = Cast<ACAttachment>(actor);
			break;
		}
	}

	Spline = CHelpers::GetComponent<UCSplineComponent>(OwnerCharacter);
	CheckNull(Spline);

	FOnTimelineFloat OnTimelineFloat;

	OnTimelineFloat.BindUFunction(this, "OnBoomerangThrowing");
	Timeline.AddInterpFloat(Spline->GetPath()->GetCurve(), OnTimelineFloat);

	FOnTimelineEvent finish;
	finish.BindUFunction(this, "OnBoomerangEnd");

	Timeline.SetTimelineFinishedFunc(finish);
}

void ACDoAction_Boomerang::DoAction()
{
	CheckFalse(Datas.Num() > 0);
	if (!!Datas[0].AnimMontage)
	{
		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	}
}

void ACDoAction_Boomerang::Begin_DoAction()
{
	CheckNull(Spline);
	boomerang->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
	boomerang->SetActorLocation(OwnerCharacter->GetActorLocation());
	float length = Spline->GetPath()->GetSpline()->GetSplineLength();
	boomerang->SetActorRotation(FRotator(90, 0, 0));
	boomerang->OnCollision();
	Timeline.SetPlayRate(1000 / length);
	Timeline.PlayFromStart();
}

void ACDoAction_Boomerang::End_DoAction()
{
	//End Action -> Set Idle, Set Move
	State->SetIdleMode();
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(action);
	action->SetSwordMode();
}

void ACDoAction_Boomerang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaTime);
}

void ACDoAction_Boomerang::OnBoomerangEnd()
{
	//캐릭터에게 돌아가는 타이머 실행
	UKismetSystemLibrary::K2_SetTimer(this, "ReturnToPlayer", timer, true);
}

void ACDoAction_Boomerang::OnBoomerangThrowing(float Output)
{
	USplineComponent* spline = Spline->GetPath()->GetSpline();
	float length = Spline->GetPath()->GetSpline()->GetSplineLength();
	FVector location = spline->GetLocationAtDistanceAlongSpline(Output * length, ESplineCoordinateSpace::World);
	FRotator deltaRotation = FRotator(0, 3600, 0) * GetWorld()->GetDeltaSeconds();

	boomerang->SetActorLocationAndRotation(location, boomerang->GetActorRotation() + deltaRotation);
}

void ACDoAction_Boomerang::ReturnToPlayer()
{
	CheckNull(Spline);
	//거리 = 속력 * 시간
	//length = speed * time
	//time = 1000/length
	//speed = 1000
	//deltatime = 1/60
	static int32 start = 1000;
	float speed = start++/Spline->GetPath()->GetSpline()->GetSplineLength();

	FVector location = boomerang->GetActorLocation();


	FVector newLocation = UKismetMathLibrary::VInterpTo(location, OwnerCharacter->GetActorLocation(), GetWorld()->GetDeltaSeconds(), speed);
	FRotator deltaRotation = FRotator(0, 3600, 0) * GetWorld()->GetDeltaSeconds();

	boomerang->SetActorLocationAndRotation(newLocation, boomerang->GetActorRotation() + deltaRotation);

	if (boomerang->GetDistanceTo(OwnerCharacter)<=75.0f)
	{
		boomerang->OnUnequip();
		Spline->Reset();
		boomerang->OffCollision();
		UKismetSystemLibrary::K2_ClearTimer(this, "ReturnToPlayer");
	}
}