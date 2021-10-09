#include "CObject_Money.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"

#include "Components/CInventoryComponent.h"

ACObject_Money::ACObject_Money()
{
	PrimaryActorTick.bCanEverTick = true;
	
	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_Money.SM_Money'");
	Mesh->SetStaticMesh(mesh);

	Mesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	Mesh->SetCollisionProfileName("OverlapAllDynamic");

	CHelpers::GetAsset(&Curve, "CurveFloat'/Game/Objects/Curve_ZeroToOne.Curve_ZeroToOne'");
}

void ACObject_Money::BeginPlay()
{
	Super::BeginPlay();

	//드랍되는 형태가 아니면
	if (bOnGround == false)
	{
		FOnTimelineFloat OnTimelineFloat;
		OnTimelineFloat.BindUFunction(this, "OnSpawned");

		FOnTimelineEvent finish;
		finish.BindUFunction(this, "FinishSpawned");

		Timeline.AddInterpFloat(Curve, OnTimelineFloat);
		Timeline.SetTimelineFinishedFunc(finish);
		Timeline.SetPlayRate(1 / ActivationTime);
		Timeline.PlayFromStart();
	}
	SetMoneyValue();
}

void ACObject_Money::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bOnGround == false)
	{
		if (Timeline.IsPlaying())
			Timeline.TickTimeline(DeltaTime);
	}
	else
	{
		AddActorWorldRotation(FRotator(0, 180, 0) * DeltaTime);
	}
}

void ACObject_Money::SetMoneyValue()
{
	float moneyArray[10] = { 10.0f,10.0f,10.0f,10.0f,10.0f,10.0f,30.0f,30.0f,30.0f,50.0f };
	int32 index = UKismetMathLibrary::RandomIntegerInRange(0, ARRAY_COUNT(moneyArray)-1);
	Value = moneyArray[index];
}

void ACObject_Money::Interact(AActor* InInteractor)
{
	CheckFalse(bOnGround);
	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(InInteractor);
	CheckNull(inventory);
	inventory->AddMoney(Value);
	Destroy();
}

void ACObject_Money::OnSpawned(float Output)
{
	AddActorWorldRotation(FRotator(0, 720, 0) * GetWorld()->GetDeltaSeconds());
	AddActorWorldOffset(FVector::UpVector * Output * MaxHeight);
}

void ACObject_Money::FinishSpawned()
{
	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	CheckNull(inventory);

	inventory->AddMoney(Value);

	Destroy();
}
