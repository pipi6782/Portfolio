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
	//Begin Play가 돌아야 Projectile이 정상적으로 돌아간다!
	Super::BeginPlay();

	OnObjectInteract.AddDynamic(this, &ACObject_Throw::Begin_Interact);
	OnObjectThrown.AddDynamic(this, &ACObject_Throw::OnThrown);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACObject_Throw::OnComponentBeginOverlap);
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
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//플레이어에게 오브젝트를 붙여준다.
	Scene->AttachToComponent(InteractedCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "Hand_Throw");
	
	//바로 던지는것을 방지하기 위해 플레이어의 HitResult를 초기화한다.
	InteractedCharacter->ResetHitResult();

	//DoAction을 할 때 Detach를 시키기 위해서 플레이어를 오너로 설정한다.
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
