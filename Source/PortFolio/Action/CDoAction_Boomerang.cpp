#include "CDoAction_Boomerang.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Character/CPlayer.h"
#include "Character/CEnemy.h"
#include "Character/CEnemy_Boss.h"
#include "Components/SplineComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CSplineComponent.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CSplinePath.h"
#include "CDoAction_Boomerang.h"
#include "CDrawLine.h"
#include "Objects/CObject_Chest.h"
#include "Objects/CObject_Heart.h"
#include "Managers/CBossManager.h"
#include "CHUD.h"


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

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACDrawLine::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		draw = Cast<ACDrawLine>(actor);
		if (!!draw) break;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBossManager::StaticClass(), actors);
	for (AActor* actor : actors)
	{
		bossManager = Cast<ACBossManager>(actor);
		if (!!bossManager) break;
	}
	
	OnBoomerang_End.AddDynamic(draw, &ACDrawLine::ResetPoints);
	OnBoomerang_End.AddDynamic(bossManager, &ACBossManager::CheckBossOnBoomerangEnd);
	OnBoomerang_End.AddDynamic(bossManager, &ACBossManager::Reset);

}

void ACDoAction_Boomerang::DoAction()
{
	CheckFalse(Datas.Num() > 0);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		player->PlayMontage(&Datas[0]);
	}
	else
	{
		OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	}
	bAction = true;
}

void ACDoAction_Boomerang::Begin_DoAction()
{
	CheckNull(Spline);
	draw->SetFlying();
	boomerang->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
	boomerang->SetActorLocation(OwnerCharacter->GetActorLocation());
	float length = Spline->GetPath()->GetSpline()->GetSplineLength();
	boomerang->OnCollision();
	Timeline.SetPlayRate(1500 / length);
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

void ACDoAction_Boomerang::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOtherActor)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherActor);
	CheckNull(InOtherActor);

	//이미 겹친 액터들은 대미지를 주지 않음
	for (const AActor* actor : DamagedActors)
	{
		if (InOtherActor == actor)
		{
			return;
		}
	}

	DamagedActors.Add(InOtherActor);

	//부딪힌 대상이 상자면 부메랑 진행 끝냄
	if (InOtherActor->IsA<ACObject_Chest>())
	{
		Abort();
		return;
	}

	//HitStop
	//float hitStop = Datas[0].HitStop;
	//FTimerHandle WaitHandle;
	//if (FMath::IsNearlyZero(hitStop) == false)
	//{
	//	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
	//	GetWorld()->GetTimerManager().SetTimer
	//	(
	//		WaitHandle,
	//		FTimerDelegate::CreateLambda([&]() //[&] 이곳에 들어오는 함수들은 전부 다 참조변수이다, 내부에서 수정 시 영향을 미침
	//			{
	//				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	//			}),
	//		hitStop * 2e-2f,
	//		false
	//	);
	//}
	
	//SendDamage
	FDamageEvent e;
	InOtherActor->TakeDamage(Datas[0].Power, e, InAttacker->GetInstigatorController(), InAttackCauser);

	ACEnemy* enemy = Cast<ACEnemy>(InOtherActor);
	if (!!enemy)
	{
		//부메랑으로 맞춘 상대가 보스라면
		if (enemy->GetName().Contains("Boss") && Timeline.IsPlaying())
		{
			CheckTrue(enemy->GetName().Contains("Final"));
			bossManager->AddBoss(Cast<ACEnemy_Boss>(enemy));
			enemy->FollowBoomerang(boomerang);
			enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			enemy->DestroyMagicBallAndStopMontage();
			UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
			CheckNull(state);
			state->SetAttachedMode();
		}
		//일반 던전의 적이라면
		else
		{
			draw->ResetPoints();
			Abort();
		}
	}
}

void ACDoAction_Boomerang::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOtherActor)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherActor);
	DamagedActors.Empty();
}

void ACDoAction_Boomerang::OnBoomerangEnd()
{
	//캐릭터에게 돌아가는 타이머 실행
	if (OnBoomerang_End.IsBound())
		OnBoomerang_End.Broadcast();
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

void ACDoAction_Boomerang::Unequip_Boomerang()
{
	boomerang->OnUnequip();
	Spline->Reset();
	boomerang->OffCollision();
	UKismetSystemLibrary::K2_ClearTimer(this, "ReturnToPlayer");
	bAction = false;
}

void ACDoAction_Boomerang::Abort()
{
	Timeline.Stop();
	//캐릭터에게 돌아가는 타이머 실행
	if (OnBoomerang_End.IsBound())
		OnBoomerang_End.Broadcast();
	UKismetSystemLibrary::K2_SetTimer(this, "ReturnToPlayer", timer, true);
}

void ACDoAction_Boomerang::ReturnToPlayer()
{
	CheckNull(Spline);

	static int32 start = 1000;
	float speed = start/Spline->GetPath()->GetSpline()->GetSplineLength();
	start += 5;

	FVector location = boomerang->GetActorLocation();


	FVector newLocation = UKismetMathLibrary::VInterpTo(location, OwnerCharacter->GetActorLocation(), GetWorld()->GetDeltaSeconds(), speed);
	FRotator deltaRotation = FRotator(0, 3600, 0) * GetWorld()->GetDeltaSeconds();

	boomerang->SetActorLocationAndRotation(newLocation, boomerang->GetActorRotation() + deltaRotation);

	if (boomerang->GetDistanceTo(OwnerCharacter)<=75.0f)
	{
		Unequip_Boomerang();
		UKismetSystemLibrary::K2_ClearTimer(this, "ReturnToPlayer");
	}
}