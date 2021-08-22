#include "CObject_Throw.h"
#include "Global.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Character/CPlayer.h"
#include "Components/CActionComponent.h"

ACObject_Throw::ACObject_Throw()
{
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");
	CHelpers::CreateComponent(this, &Box, "Box", Scene);
	CHelpers::CreateComponent(this, &Destructible, "Destructible", Scene);

	UDestructibleMesh* mesh;
	CHelpers::GetAsset(&mesh, "DestructibleMesh'/Game/Meshes/DM_Urn.DM_Urn'");
	Destructible->SetDestructibleMesh(mesh);
	Destructible->SetRelativeScale3D(FVector(2, 2, 2));
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = false;

	Projectile->ProjectileGravityScale = 0.0f;

	Box->SetRelativeLocation(FVector(0, 0, 50));
	Box->SetBoxExtent(FVector(60, 60, 50));
}

void ACObject_Throw::BeginPlay()
{
	//Begin Play�� ���ƾ� Projectile�� ���������� ���ư���!
	Super::BeginPlay();

	OnObjectInteract.AddDynamic(this, &ACObject_Throw::Begin_Interact);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::OnThrown);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACObject_Throw::OnComponentBeginOverlap);
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
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//�÷��̾�� ������Ʈ�� �ٿ��ش�.
	Scene->AttachToComponent(InteractedCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "Hand_Throw");
	
	//�ٷ� �����°��� �����ϱ� ���� �÷��̾��� HitResult�� �ʱ�ȭ�Ѵ�.
	InteractedCharacter->ResetHitResult();

	//DoAction�� �� �� Detach�� ��Ű�� ���ؼ� �÷��̾ ���ʷ� �����Ѵ�.
	SetOwner(InteractedCharacter);
}

void ACObject_Throw::OnThrown()
{
	SetActorRotation(InteractedCharacter->GetActorForwardVector().Rotation());
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 300, FLinearColor::Black, 10.0f, 5.0f);
	Destructible->GetDestructibleMesh()->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = true;
	Destructible->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Projectile->SetVelocityInLocalSpace(FVector(1,0,0) * 800);
	Projectile->MaxSpeed = 20000.0f;
	Projectile->ProjectileGravityScale = 1.0f;
}

void ACObject_Throw::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == this);
	//CheckTrue
}
