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
	//Begin Play�� ���ƾ� Projectile�� ���������� ���ư���!
	Super::BeginPlay();

	OnObjectInteract.AddDynamic(this, &ACObject_Throw::Begin_Interact);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::DetachActor);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::OnThrown);
	Destructible->OnComponentHit.AddDynamic(this, &ACObject_Throw::OnComponentHit);
}

void ACObject_Throw::Begin_Interact(ACharacter* InCharacter)
{
	//�̹� ��ȣ�ۿ��� ������Ʈ���� �˻�
	CheckTrue(bInteracted);
	bInteracted = true;
	
	//��ȣ�ۿ��� �õ��� ĳ���Ͱ� ��ȿ���� �˻�
	CheckNull(Cast<ACPlayer>(InCharacter));
	InteractedCharacter = Cast<ACPlayer>(InCharacter);

	//Throwing Mode�� �ٲٱ� ���� Action Component�� �����´�.
	//������Ʈ�� Null���� �ƴϸ� Throwing Mode�� �ٲ��ش�.
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(InteractedCharacter);
	CheckNull(action);
	action->SetThrowingMode();
	
	//ĳ���͸� ������Ʈ ������ ������.
	FVector characterVector = InteractedCharacter->GetActorLocation();
	characterVector = FVector(characterVector.X, characterVector.Y, 0);
	FVector objectVector = GetActorLocation();
	objectVector = FVector(objectVector.X, objectVector.Y, 0);
	InteractedCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(characterVector, objectVector));

	//ĳ���Ϳ� ������Ʈ�� ��ġ�� �κ��� ������ ������Ʈ�� �ݸ����� ����.
	Destructible->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//�÷��̾�� ������Ʈ�� �ٿ��ش�.
	SetActorLocation(FVector(0, 0, 0));
	Destructible->AttachToComponent(InteractedCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Hand_Throw");
	
	//�ٷ� �����°��� �����ϱ� ���� �÷��̾��� HitResult�� �ʱ�ȭ�Ѵ�.
	InteractedCharacter->ResetHitResult();

	//DoAction�� �� �� Detach�� ��Ű�� ���ؼ� �÷��̾ ���ʷ� �����Ѵ�.
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
			FTimerDelegate::CreateLambda([&]() //[&] �̰��� ������ �Լ����� ���� �� ���������̴�, ���ο��� ���� �� ������ ��ħ
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
			FTimerDelegate::CreateLambda([&]() //[&] �̰��� ������ �Լ����� ���� �� ���������̴�, ���ο��� ���� �� ������ ��ħ
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
