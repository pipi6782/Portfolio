#include "CObject_Throw.h"
#include "Global.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Character/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Action/CAttachment.h"
#include "CObject_Heart.h"

ACObject_Throw::ACObject_Throw()
{
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");
	CHelpers::CreateComponent(this, &Destructible, "Destructible", Scene);

	UDestructibleMesh* mesh;
	CHelpers::GetAsset(&mesh, "DestructibleMesh'/Game/Meshes/DM_Urn.DM_Urn'");
	Destructible->SetDestructibleMesh(mesh);
	Destructible->SetRelativeScale3D(FVector(2, 2, 2));
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = false;

	Projectile->ProjectileGravityScale = 0.0f;

}

void ACObject_Throw::BeginPlay()
{
	//Begin Play가 돌아야 Projectile이 정상적으로 돌아간다!
	Super::BeginPlay();

	OnObjectInteract.AddDynamic(this, &ACObject_Throw::Begin_Interact);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::DetachActor);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::OnThrown);
	Destructible->OnComponentHit.AddDynamic(this, &ACObject_Throw::OnComponentHit);
}

void ACObject_Throw::Begin_Interact(ACharacter* InCharacter)
{
	//이미 상호작용한 오브젝트인지 검사
	CheckTrue(bInteracted);
	bInteracted = true;
	
	//상호작용을 시도한 캐릭터가 유효한지 검사
	CheckNull(Cast<ACPlayer>(InCharacter));
	InteractedCharacter = Cast<ACPlayer>(InCharacter);

	//Throwing Mode로 바꾸기 위해 Action Component를 가져온다.
	//컴포넌트가 Null값이 아니면 Throwing Mode로 바꿔준다.
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(InteractedCharacter);
	CheckNull(action);
	action->SetThrowingMode();
	
	//캐릭터를 오브젝트 쪽으로 돌린다.
	FVector characterVector = InteractedCharacter->GetActorLocation();
	characterVector = FVector(characterVector.X, characterVector.Y, 0);
	FVector objectVector = GetActorLocation();
	objectVector = FVector(objectVector.X, objectVector.Y, 0);
	InteractedCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(characterVector, objectVector));

	//캐릭터와 오브젝트가 겹치는 부분이 있으니 오브젝트의 콜리전을 끈다.
	Destructible->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//플레이어에게 오브젝트를 붙여준다.
	SetActorLocation(FVector(0, 0, 0));
	Destructible->AttachToComponent(InteractedCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Throw");
	
	//바로 던지는것을 방지하기 위해 플레이어의 HitResult를 초기화한다.
	InteractedCharacter->ResetHitResult();

	//DoAction을 할 때 Detach를 시키기 위해서 플레이어를 오너로 설정한다.
	SetOwner(InteractedCharacter);
	CLog::Log(GetName()+ ", " + Destructible->GetOwner()->GetName());
}

void ACObject_Throw::OnThrown()
{
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = true;
	Destructible->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Projectile->SetVelocityInLocalSpace(FVector(1,0,0) * 800);
	Destructible->SetSimulatePhysics(true);

}

void ACObject_Throw::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CheckTrue(OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckTrue(OtherActor->GetName().Compare(GetName()) == 0);
	CheckTrue(OtherActor->IsA<ACAttachment>());
	CheckNull(GetOwner());

	Destructible->ApplyDamage(1000.0f, Hit.ImpactPoint, FVector(0, 0, 1), 1.0f);

	FDamageEvent e;
	
	CLog::Print(GetOwner()->GetName() + ", " + GetOwner()->GetInstigatorController()->GetName() );
	OtherActor->TakeDamage(1000.0f, e, GetOwner()->GetInstigatorController(), this);

	float WaitTime = 1.5f;
	FTimerHandle WaitHandle;
	if (bDamaged == false)
	{
		bDamaged = true;
		GetWorld()->GetTimerManager().SetTimer
		(
			WaitHandle,
			FTimerDelegate::CreateLambda([&]() //[&] 이곳에 들어오는 함수들은 전부 다 참조변수이다, 내부에서 수정 시 영향을 미침
				{
					Destroy();
				}),
			WaitTime,
			false
		);

		SpawnObject(Hit.ImpactPoint);
	}


}

void ACObject_Throw::DetachActor()
{
	Destructible->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
}

void ACObject_Throw::SpawnObject(FVector InLocation)
{
	FTransform transform;
	transform.SetLocation(InLocation + FVector(0,0,25));
	transform.SetScale3D(FVector(0.7f, 0.7f, 0.7f));

	GetWorld()->SpawnActor<ACObject_Heart>
		(
			ACObject_Heart::StaticClass(),
			transform
		);
}

float ACObject_Throw::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckTrueResult(bDamaged,0.0f);
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Destructible->ApplyDamage(damage, GetActorLocation(), FVector(0, 0, 1), 10.0f);

	FTimerHandle WaitHandle;
	float WaitTime = 1.5f;
	if (bDamaged == false)
	{
		bDamaged = true;
		GetWorld()->GetTimerManager().SetTimer
		(
			WaitHandle,
			FTimerDelegate::CreateLambda([&]() //[&] 이곳에 들어오는 함수들은 전부 다 참조변수이다, 내부에서 수정 시 영향을 미침
			{
				if(!!GetOwner())
					SetOwner(nullptr);
				Destroy();
			}),
			WaitTime,
			false
		);

		SpawnObject(GetActorLocation());
	}

	return damage;
}
