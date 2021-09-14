#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/WidgetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CActionComponent.h"

ACEnemy::ACEnemy()
{
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

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;



	//대미지를 준 대상이 부메랑인지 체크
	if (DamageCauser->GetName().Contains("Boomerang") || DamageCauser->GetName().Contains("Throw"))
	{
		Status->SubHealth(0.0f);
	}
	else
	{
		//캐릭터가 적의 전방을 공격했는지 체크
		float dotResult = GetDotProductTo(DamageInstigator->GetPawn());
		if (dotResult <= 0.0f)
		{
			Status->SubHealth(0.0f);
		}
		else
		{
			Status->SubHealth(damage);
		}
	}

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}
	
	State->SetDamagedMode();
	CLog::Print(Status->GetHealth());
	return Status->GetHealth();
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();
	//TODO : 해당 레벨 리셋작업 하기
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