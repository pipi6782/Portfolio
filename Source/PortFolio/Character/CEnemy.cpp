#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Character/CAIController.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Action/CThrow.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");

	//Component Settings
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> animClass;
	CHelpers::GetClass<UAnimInstance>(&animClass, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animClass);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFollow == true)
	{
		CheckNull(followAttachment);
		FVector location = followAttachment->GetActorLocation();
		location = FVector(location.X, location.Y, GetActorLocation().Z);
		SetActorLocation(location);
		ACAIController* controller = Cast<ACAIController>(GetController());
		if (!!controller)
		{
			UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
			if (!!behavior)
			{
				behavior->SetWaitMode();
			}
		}
	}
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();
	//TODO : 해당 레벨 리셋작업 하기
}

void ACEnemy::DestroyMagicBallAndStopMontage()
{
	for (AActor* actor : Children)
	{
		ACThrow* throwObject = Cast<ACThrow>(actor);
		if (!!throwObject)
		{
			throwObject->Destroy();
			StopAnimMontage();
			ACAIController* controller = Cast<ACAIController>(GetController());
			if (!!controller)
			{
				controller->DisableAttack();
				UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
				if (!!behavior)
				{
					behavior->SetWaitMode();
				}
			}

			break;
		}
	}
}

void ACEnemy::FollowBoomerang(AActor* InAttachment)
{
	CheckNull(InAttachment);
	bFollow = true;
	followAttachment = InAttachment;
	CLog::Log("Attached To Boomerang");
}

void ACEnemy::UnfollowBoomerang()
{
	followAttachment = nullptr;
	bFollow = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CLog::Log("Detached To Boomerang");
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Damaged: Damaged();	break;
		case EStateType::Dead: Dead();	break;
	}
}

void ACEnemy::Damaged()
{
	//Set Move
	Status->SetMove();

	//Play Damaged Montage
	Montages->PlayDamaged();

	//플레이어쪽으로 돌아보게하기
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Montages->PlayDead();
}