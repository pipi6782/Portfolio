#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CHUD.h"
#include "CAnimInstance.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CSplineComponent.h"
#include "Components/CDrawRouteComponent.h"
#include "Components/CInventoryComponent.h"
#include "Objects/CObject.h"
#include "Objects/CObject_Potion.h"
#include "Widgets/CUserWidget_Health.h"
#include "Widgets/CUserWidget_Weapon.h"
#include "Widgets/CUserWidget_Inventory.h"
#include "Interfaces/IInteractItem.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	{
		CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
		CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
		CHelpers::CreateComponent(this, &Pitching, "Pitching", GetMesh());
		CHelpers::CreateComponent(this, &OutlineMesh, "OutlineMesh", GetCapsuleComponent());
	}
	
	//Create ActorComponent
	{
		CHelpers::CreateActorComponent(this, &Status, "Status");
		CHelpers::CreateActorComponent(this, &State, "State");
		CHelpers::CreateActorComponent(this, &Action, "Action");
		CHelpers::CreateActorComponent(this, &Montage, "Montage");
		CHelpers::CreateActorComponent(this, &Spline, "Spline");
		CHelpers::CreateActorComponent(this, &Draw, "Draw");
		CHelpers::CreateActorComponent(this, &Inventory, "Inventory");
	}

	//Mesh Setting
	{
		{
			UStaticMesh* mesh;
			CHelpers::GetAsset(&mesh, "StaticMesh'/Game/AncientTreasures/Meshes/SM_Helmet_01a.SM_Helmet_01a'");
			Pitching->SetStaticMesh(mesh);
			Pitching->SetCollisionProfileName("OverlapAllDynamic");
			Pitching->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Pitching");
		}

		{
			USkeletalMesh* mesh;
			CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Natural.SK_CharM_Natural'");
			GetMesh()->SetSkeletalMesh(mesh);
			GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
			GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
			TSubclassOf<UCAnimInstance> animClass;
			CHelpers::GetClass(&animClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
			GetMesh()->SetAnimClass(animClass);
		}
	}

	//CapsuleComponent Setting
	{
		GetCapsuleComponent()->InitCapsuleSize(45.0f, 96.0f);
	}

	//Character Setting
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
	}

	//CharacterMovement Setting
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0, 640, 0);
		GetCharacterMovement()->bConstrainToPlane = true;
		GetCharacterMovement()->bSnapToPlaneAtStart = true;
	}
	
	//SpringArm Setting
	{
		SpringArm->SetUsingAbsoluteRotation(true);
		SpringArm->TargetArmLength = 1000.0f;
		SpringArm->SetRelativeRotation(FRotator(-60, 90, 0));
		SpringArm->bDoCollisionTest = false;
		SpringArm->bEnableCameraLag = true;
	}
	
	//Camera Setting
	{
		Camera->bUsePawnControlRotation = false;
	}

	//Widget Setting
	{
		CHelpers::GetClass(&HealthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
		CHelpers::GetClass(&WeaponWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Weapon.WB_Weapon_C'");
		CHelpers::GetClass(&InventoryWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Inventory.WB_Inventory_C'");
	}

	//OutlineMesh Setting
	{
		USkeletalMesh* mesh;
		CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Natural.SK_CharM_Natural'");
		OutlineMesh->SetSkeletalMesh(mesh);
		OutlineMesh->SetRelativeLocation(FVector(0, 0, -88));
		OutlineMesh->SetRelativeRotation(FRotator(0, -90, 0));
		OutlineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TSubclassOf<UCAnimInstance> animClass;
		CHelpers::GetClass(&animClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
		OutlineMesh->SetAnimClass(animClass);
	}
}

void ACPlayer::OnConstruction(const FTransform& Transform)
{
	//Material Setting
	UMaterialInstanceConstant* lineMaterial;
	CHelpers::GetAssetDynamic(&lineMaterial, "MaterialInstanceConstant'/Game/InfinityBladeWarriors/Character/CompleteCharacters/Textures_Materials/CharM_Natural/MI_PlayerOutline.MI_PlayerOutline'");
	OutlineMaterial = UMaterialInstanceDynamic::Create(lineMaterial, this);
	OutlineMesh->SetMaterial(0, OutlineMaterial);
}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Event Binding
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	//Mouse Setting
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerController->DefaultMouseCursor = EMouseCursor::CardinalCross;
	playerController->bShowMouseCursor = true;

	//Create Widget
	HealthWidget = CreateWidget<UCUserWidget_Health, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HealthWidgetClass);
	HealthWidget->AddToViewport();
	HealthWidget->Initialize();
	HealthWidget->SetVisibility(ESlateVisibility::Visible);

	HealthWidget->UpdateMaxHealth(Status->GetMaxHealth());
	HealthWidget->UpdateHealth(Status->GetHealth());

	WeaponWidget = CreateWidget<UCUserWidget_Weapon, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WeaponWidgetClass);
	WeaponWidget->AddToViewport();
	WeaponWidget->Initialize();
	WeaponWidget->SetVisibility(ESlateVisibility::Visible);

	InventoryWidget = CreateWidget<UCUserWidget_Inventory, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->Initialize();
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);


	//Binding Collision Event
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnComponentBeginOverlap);

	//SetTeamID
	SetGenericTeamId(FGenericTeamId(TeamID));
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Draw Boomerang Route
	if (Action->IsBoomerangMode())
	{
		if (bClicked)
		{
			Spline->DrawRoute();
			Draw->DrawLine();
		}
	}	
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ACPlayer::OffAction);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACPlayer::OnInteract);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ACPlayer::OnInventory);
}

void ACPlayer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	IIInteractItem* item = Cast<IIInteractItem>(OtherActor);
	CheckNull(item);
	item->Interact(this);
}

void ACPlayer::OnAction()
{
	CheckNull(State);
	CheckNull(Status);
	CheckTrue(Inventory->IsUsingInventory());

	if (Action->IsUnarmedMode())
	{
		Action->SetSwordMode();
	}

	if (Action->IsSwordMode() || Action->IsThrowingMode())
	{
		Action->DoAction();
	}
	else if(Action->IsBoomerangMode())
	{
		bClicked = true;
	}
}

void ACPlayer::OffAction()
{
	CheckNull(State);
	CheckNull(Status);
	CheckFalse(State->IsIdleMode());
	
	if(Action->IsBoomerangMode())
	{
		Action->DoAction();
		bClicked = false;
	}
}

void ACPlayer::OnInteract()
{
	TArray<FHitResult> OutHits;
	TraceObject(OutHits);
	CheckTrue(OutHits.Num() == 0);
	CheckFalse((Action->IsUnarmedMode() || Action->IsSwordMode()));
	CheckTrue(Inventory->IsUsingInventory());

	CheckFalse(State->IsIdleMode());

	for (FHitResult hitResult : OutHits)
	{
		ACObject* object = Cast<ACObject>(hitResult.Actor);
		if (!!object)
		{
			object->OnObjectInteract.Broadcast(this);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();
			break;
		}
	}
}

void ACPlayer::OnInventory()
{
	CheckFalse(State->IsIdleMode());
	Inventory->ToggleInventory();
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());
	CheckTrue(Inventory->IsUsingInventory());
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();
	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());
	CheckTrue(Inventory->IsUsingInventory());
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();
	AddMovementInput(direction, InAxis);
}

//void ACPlayer::MoveToDestination()
//{
//	//마우스를 클릭한 곳으로 이동
//	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, HitResult.ImpactPoint);
//}
//
//void ACPlayer::SetDestination()
//{
//	if(Action->IsBoomerangMode() == false)
//		CheckFalse(Status->CanMove());
//	ClickTime += GetWorld()->GetDeltaSeconds();
//
//	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//
//	TArray<TEnumAsByte<EObjectTypeQuery>> quries;
//	quries.Add(EObjectTypeQuery::ObjectTypeQuery1);
//	quries.Add(EObjectTypeQuery::ObjectTypeQuery2);
//	quries.Add(EObjectTypeQuery::ObjectTypeQuery6);
//
//	playerController->GetHitResultUnderCursorForObjects(quries, true, HitResult);
//
//	float distance = FVector::Dist2D(GetActorLocation(), HitResult.ImpactPoint);
//	distance = FMath::Clamp(distance, 0.0f, SprintDistance);
//
//	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(0.0f,Status->GetSprintSpeed(),distance/ SprintDistance);
//}

void ACPlayer::TraceObject(TArray<FHitResult>& OutHits)
{
	CheckFalse((Action->IsUnarmedMode() || Action->IsSwordMode()));

	TArray<TEnumAsByte<EObjectTypeQuery>> quries;
	quries.Add(EObjectTypeQuery::ObjectTypeQuery2);
	quries.Add(EObjectTypeQuery::ObjectTypeQuery6);

	TArray<AActor*> ignores;
	ignores.Add(this);
	FVector start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 20.0f);
	FVector end = start + GetActorForwardVector() * 100;
	UKismetSystemLibrary::LineTraceMultiForObjects
	(
		GetWorld(),
		start,
		end,
		quries,
		false,
		ignores,
		EDrawDebugTrace::None,
		OutHits,
		true
	);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::Damaged()
{
	//TODO : 차후에 포스트프로세스 넣을경우 수정

	Status->SetMove();
	Montage->PlayDamaged();
}

void ACPlayer::Dead()
{
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montage->PlayDead();
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	//TODO : 해당 레벨 재시작 할 수 있게 수정
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;

	Status->SubHealth(damage);

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0.0f;
	}

	State->SetDamagedMode();

	return Status->GetHealth();
}

void ACPlayer::EnableHidden()
{
	bHideInZone = true;
	OutlineMaterial->SetScalarParameterValue("Intensity", 2.0f);
	OutlineMaterial->SetScalarParameterValue("Width", 10.0f);
}

void ACPlayer::DisableHidden()
{
	bHideInZone = false;
	OutlineMaterial->SetScalarParameterValue("Intensity", 0.0f);
	OutlineMaterial->SetScalarParameterValue("Width", 0.0f);
}

void ACPlayer::PlayMontage(const FMontageData* InData)
{
	PlayAnimMontage(InData->AnimMontage, InData->PlayRatio, InData->StartSection);
	//OutlineMesh도 똑같은 몽타주를 재생시킴
	UAnimInstance* AnimInstance = (OutlineMesh) ? OutlineMesh->GetAnimInstance() : nullptr;
	if (InData->AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(InData->AnimMontage, InData->PlayRatio);

		if (Duration > 0.0f)
		{
			// Start at a given Section->
			if (InData->StartSection != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(InData->StartSection, InData->AnimMontage);
			}
		}
	}
}

void ACPlayer::PlayMontage(const FDoActionData* InData)
{
	PlayAnimMontage(InData->AnimMontage, InData->PlayRate, InData->StartSection);
	//OutlineMesh도 똑같은 몽타주를 재생시킴
	UAnimInstance* AnimInstance = (OutlineMesh) ? OutlineMesh->GetAnimInstance() : nullptr;
	if (InData->AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(InData->AnimMontage, InData->PlayRate);

		if (Duration > 0.0f)
		{
			// Start at a given Section->
			if (InData->StartSection != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(InData->StartSection, InData->AnimMontage);
			}
		}
	}
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Damaged:
			Damaged();
			break;
		case EStateType::Dead:
			Dead();
			break;
	}
}
