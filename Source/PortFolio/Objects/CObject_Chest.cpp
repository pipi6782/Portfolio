#include "CObject_Chest.h"
#include "Global.h"
#include "Components/BoxComponent.h"

#include "Character/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "CObject_Life.h"

ACObject_Chest::ACObject_Chest()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Top, "Top", Scene);
	CHelpers::CreateComponent(this, &Bottom, "Bottom", Scene);
	CHelpers::CreateComponent(this, &Box, "Box", Scene);
	
	//Box Setting
	Box->SetBoxExtent(FVector(100, 100, 80));

	//Mesh Setting
	UStaticMesh* mesh;
	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_ChestTop.SM_ChestTop'");
	Top->SetStaticMesh(mesh);

	CHelpers::GetAsset(&mesh, "StaticMesh'/Game/Meshes/SM_ChestBottom.SM_ChestBottom'");
	Bottom->SetStaticMesh(mesh);


	Top->SetRelativeLocation(FVector(-45, 0, 0));
	Bottom->SetRelativeLocation(FVector(-45, 0, 0));
	CHelpers::GetAsset(&Curve, "CurveFloat'/Game/Objects/Curve_BoxPitch.Curve_BoxPitch'");
}

void ACObject_Chest::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindUFunction(this, "Opening");

	Timeline.AddInterpFloat(Curve, OnTimelineFloat);

	FOnTimelineEvent OnTimelineEvent;
	OnTimelineEvent.BindUFunction(this, "EndOpening");

	Timeline.SetTimelineFinishedFunc(OnTimelineEvent);

	Timeline.SetPlayRate(OpeningSpeed);

	OnObjectInteract.AddDynamic(this, &ACObject_Chest::Begin_Interact);
}

void ACObject_Chest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaSeconds);
}

void ACObject_Chest::Begin_Interact(class ACharacter* InCharacter)
{
	//�̹� ��ȣ�ۿ� �ߴ��� �˻�
	CheckTrue(bInteracted);
	bInteracted = true;
	//��ȣ�ۿ��� ĳ���Ͱ� �÷��̾����� �˻�
	CheckNull(Cast<ACPlayer>(InCharacter));
	InteractedCharacter = Cast<ACPlayer>(InCharacter);

	//Interacting Mode�� �ٲٱ� ���� State Component�� �����´�.
	//������Ʈ�� Null���� �ƴϸ� Interacting Mode�� �ٲ��ش�.
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InteractedCharacter);
	CheckNull(state);
	state->SetInteractingMode();

	//�������� ���ϰ� �ϱ� ���� Status Component�� �����´�.
	//������Ʈ�� Null���� �ƴϸ� �������� ���ϰ� �ٲ��ش�.
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InteractedCharacter);
	CheckNull(status);
	status->SetStop();

	//ĳ���͸� ������Ʈ ������ ������.
	FVector characterVector = InteractedCharacter->GetActorLocation();
	characterVector = FVector(characterVector.X, characterVector.Y, 0);
	FVector objectVector = GetActorLocation();
	objectVector = FVector(objectVector.X, objectVector.Y, 0);
	InteractedCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(characterVector, objectVector));

	//ĳ������ HitResult�� �ʱ�ȭ�Ѵ�.
	InteractedCharacter->ResetHitResult();

	//��� �۾��� �������� Ÿ�Ӷ����� �����Ų��.
	Timeline.PlayFromStart();
}

void ACObject_Chest::Opening(float Output)
{
	float pitch = UKismetMathLibrary::Lerp(0, 130, Output);
	FRotator rotation = FRotator(pitch, 0, 0);
	Top->SetRelativeRotation(rotation);
}

void ACObject_Chest::EndOpening()
{
	//���� ��ȣ�ۿ��� ���� ���� ���·� �����ߴٸ� �Լ��� �����Ű�� ����
	CheckFalse(bInteracted);

	//Idle Mode�� �ٲٱ� ���� State Component�� �����´�.
	//������Ʈ�� Null���� �ƴϸ� Idle Mode�� �ٲ��ش�.
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InteractedCharacter);
	CheckNull(state);
	state->SetIdleMode();

	//���ڿ��� ������Ʈ�� ������Ų��.
	GetWorld()->SpawnActor<ACObject_Life>(ACObject_Life::StaticClass(),GetActorLocation(),FRotator(0,0,0));
}
