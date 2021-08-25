#include "CObject_Throw.h"
#include "Global.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/Brush.h"

#include "Character/CPlayer.h"
#include "Components/CActionComponent.h"
#include "Action/CAttachment.h"

ACObject_Throw::ACObject_Throw()
{
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");
	//CHelpers::CreateComponent(this, &Box, "Box", Scene);
	CHelpers::CreateComponent(this, &Destructible, "Destructible", Scene);

	UDestructibleMesh* mesh;
	CHelpers::GetAsset(&mesh, "DestructibleMesh'/Game/Meshes/DM_Urn.DM_Urn'");
	Destructible->SetDestructibleMesh(mesh);
	Destructible->SetRelativeScale3D(FVector(2, 2, 2));
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = false;

	Projectile->ProjectileGravityScale = 0.0f;

	//Box->SetRelativeLocation(FVector(0, 0, 50));
	//Box->SetBoxExtent(FVector(60, 60, 50));
}

void ACObject_Throw::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


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
	//Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	SetOwner(nullptr);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 300, FLinearColor::Black, 10.0f, 5.0f);
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = true;
	Destructible->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Projectile->SetVelocityInLocalSpace(FVector(1,0,0) * 800);
	Destructible->SetSimulatePhysics(true);

}

void ACObject_Throw::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CheckTrue(OtherActor == InteractedCharacter);
	CheckTrue(OtherActor == this);
	CheckTrue(OtherActor->IsA<ACAttachment>());
	Destructible->ApplyDamage(10.0f, Hit.ImpactPoint, FVector(0, 0, 1), 1.0f);

	FDamageEvent e;

	OtherActor->TakeDamage(10.0f, e, GetInstigatorController(), this);
}

void ACObject_Throw::DetachActor()
{
	Destructible->SetRelativeLocation(Destructible->GetRelativeLocation() * 2);
	
	CLog::Log(GetName()+ ", " + Destructible->GetOwner()->GetName());
	
	Destructible->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
}

float ACObject_Throw::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckTrueResult(bDamaged,0.0f);
	bDamaged = true;
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	

	return damage;
}
