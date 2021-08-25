#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "CAnimInstance.h"

#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontageComponent.h"
#include "Components/CSplineComponent.h"
#include "Components/CDrawRouteComponent.h"
#include "Objects/CObject.h"
#include "Widgets/CUserWidget_Health.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	{
		CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
		CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
		CHelpers::CreateComponent(this, &Pitching, "Pitching", GetMesh());
	}
	
	//Create ActorComponent
	{
		CHelpers::CreateActorComponent(this, &Status, "Status");
		CHelpers::CreateActorComponent(this, &State, "State");
		CHelpers::CreateActorComponent(this, &Action, "Action");
		CHelpers::CreateActorComponent(this, &Montage, "Montage");
		CHelpers::CreateActorComponent(this, &Spline, "Spline");
		CHelpers::CreateActorComponent(this, &Draw, "Draw");
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
			CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
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
	}
}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//Mouse Setting
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerController->bShowMouseCursor = true;
	playerController->DefaultMouseCursor = EMouseCursor::CardinalCross;

	//Create Widget
	HealthWidget = CreateWidget<UCUserWidget_Health, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), HealthWidgetClass);
	HealthWidget->AddToViewport();
	HealthWidget->Initialize();
	HealthWidget->SetVisibility(ESlateVisibility::Visible);

	HealthWidget->UpdateMaxHealth(Status->GetMaxHealth());
	HealthWidget->UpdateHealth(Status->GetHealth());
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetDestination();
	TraceObject();

	float distance = FVector::Dist2D(GetActorLocation(), HitResult.ImpactPoint);
	//Move
	if (Action->IsBoomerangMode() == false)
	{
		if (Status->CanMove() && State->IsIdleMode())
		{
			MoveToDestination();
		}
	}

	//Draw Boomerang Route
	else if (Action->IsBoomerangMode())
	{
		if (bClicked)
		{
			//TODO : 라인 그리기
			FVector pointLocation = FVector(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, GetActorLocation().Z);
			Spline->UpdateSplineRoute(pointLocation);
			Draw->DrawLine(pointLocation);
		}
	}

	//Interact With Object
	if (OutHits.Num() > 0)
	{
		if (HitResult.Actor!=nullptr)
		{
			if (HitResult.Actor->IsA<ACObject>()) {
				for (FHitResult hitResult : OutHits)
				{
					ACObject* object = Cast<ACObject>(hitResult.Actor);
					if (!!object)
					{
						if (Action->IsUnarmedMode() || Action->IsSwordMode())
						{
							if (State->IsIdleMode())
							{
								object->OnObjectInteract.Broadcast(this);
								UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();
								Status->SetStop();
							}
						}
					}
				}
			}
		}
	}
	
	//Throwing Object
	if (Action->IsThrowingMode())
	{
		CheckNull(HitResult.Actor);
		if (ClickTime == 0.0f)
		{
			if (distance < ThrowDistance)
			{
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();
				Action->DoAction();
			}
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ACPlayer::OnAction);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ACPlayer::OffAction);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ACPlayer::OnChangeWeapon);
}

void ACPlayer::OnAction()
{
	CheckNull(State);
	CheckNull(Status);
	CheckFalse(State->IsIdleMode());


	if (Action->IsUnarmedMode() || Action->IsSwordMode() || Action->IsThrowingMode())
	{
		//TODO : 화면 터치상태에 따른 행동방식 변경 구현
		Status->SetMove();
	}
	else
	{
		bClicked = true;
	}
	
}

void ACPlayer::OffAction()
{
	CheckNull(State);
	CheckNull(Status);
	CheckFalse(State->IsIdleMode());
	
	if (Action->IsUnarmedMode() || Action->IsSwordMode() || Action->IsThrowingMode())
	{
		Status->SetStop();
	}
	else
	{
		Action->DoAction();
		bClicked = false;
	}
	ClickTime = 0.0f;
}

void ACPlayer::OnChangeWeapon()
{
	if (Action->IsBoomerangMode())
	{
		Action->SetSwordMode();
		return;
	}
	//TODO : 선택한 무기로 무기 전환
	Action->SetBoomerangMode();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();
	Status->SetStop();
}

void ACPlayer::MoveToDestination()
{
	//마우스를 클릭한 곳으로 이동
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(playerController, HitResult.ImpactPoint);
}

void ACPlayer::SetDestination()
{
	if(Action->IsBoomerangMode() == false)
		CheckFalse(Status->CanMove());
	ClickTime += GetWorld()->GetDeltaSeconds();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> quries;
	quries.Add(EObjectTypeQuery::ObjectTypeQuery1);
	quries.Add(EObjectTypeQuery::ObjectTypeQuery2);
	quries.Add(EObjectTypeQuery::ObjectTypeQuery6);

	playerController->GetHitResultUnderCursorForObjects(quries, true, HitResult);

	float distance = FVector::Dist2D(GetActorLocation(), HitResult.ImpactPoint);
	distance = FMath::Clamp(distance, 0.0f, 600.0f);

	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(0.0f,600.0f,distance/600.0f);
}

void ACPlayer::TraceObject()
{
	CheckFalse((Action->IsUnarmedMode() || Action->IsSwordMode()));

	TArray<TEnumAsByte<EObjectTypeQuery>> quries;
	quries.Add(EObjectTypeQuery::ObjectTypeQuery2);
	quries.Add(EObjectTypeQuery::ObjectTypeQuery6);

	TArray<AActor*> ignores;
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

void ACPlayer::DrawObjectLine()
{

}

