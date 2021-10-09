#include "CObject_Life.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/Character.h"

#include "Components/CStatusComponent.h"

ACObject_Life::ACObject_Life()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent(this, &LifeFront, "LifeFront", Scene);
	CHelpers::CreateComponent(this, &LifeMain, "LifeMain", Scene);
	CHelpers::CreateComponent(this, &LifeBehind, "LifeBehind", Scene);

	//Mesh Setting
	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_Plane.SM_Plane'");
	LifeFront->SetStaticMesh(mesh);
	LifeMain->SetStaticMesh(mesh);
	LifeBehind->SetStaticMesh(mesh);

	LifeFront->SetRelativeLocation(FVector(0, 1, 50));
	LifeFront->SetRelativeRotation(FRotator(0, 0, 90.0f));
	LifeFront->SetRelativeScale3D(FVector(0.7f,  0.7f, 0.7f));

	LifeMain->SetRelativeLocation(FVector(0, 0, 50));
	LifeMain->SetRelativeRotation(FRotator(0, 0, 90.0f));
	LifeMain->SetRelativeScale3D(FVector(1.1f*0.7f, 1.1f * 0.7f, 1.1f * 0.7f));

	LifeBehind->SetRelativeLocation(FVector(0, -1, 50));
	LifeBehind->SetRelativeRotation(FRotator(0, 0, 90.0f));
	LifeBehind->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset(&material, "MaterialInstanceConstant'/Game/Materials/MI_Heart.MI_Heart'");
	LifeFront->SetMaterial(0, material);
	LifeBehind->SetMaterial(0, material);

	CHelpers::GetAsset(&material, "MaterialInstanceConstant'/Game/Materials/MI_HeartBorder.MI_HeartBorder'");
	LifeMain->SetMaterial(0, material);

	//Get Asset
	CHelpers::GetAsset(&Curve, "CurveFloat'/Game/Objects/Curve_ZeroToOne.Curve_ZeroToOne'");
}

void ACObject_Life::BeginPlay()
{
	Super::BeginPlay();

	//Timeline Setting
	FOnTimelineFloat onTimelineFloat;
	onTimelineFloat.BindUFunction(this,"OnLifeSpawned");

	FOnTimelineEvent onTimelineEvent;
	onTimelineEvent.BindUFunction(this, "FinishSpawned");

	Timeline.AddInterpFloat(Curve, onTimelineFloat);
	Timeline.SetTimelineFinishedFunc(onTimelineEvent);

	Timeline.SetPlayRate(1 / ActivationTime);

	//스폰되자마자 실행
	Timeline.PlayFromStart();
}

void ACObject_Life::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaSeconds);
}

void ACObject_Life::OnLifeSpawned(float Output)
{
	AddActorWorldRotation(FRotator(0, 720, 0) * GetWorld()->GetDeltaSeconds());
	AddActorWorldOffset(FVector::UpVector * Output * MaxHeight);
}

void ACObject_Life::FinishSpawned()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	CheckNull(character);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(character);

	CheckNull(character);

	status->AddMaxHealth();

	Destroy();
}