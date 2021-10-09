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

	CHelpers::GetAsset(&Table, "DataTable'/Game/Datas/DT_Reward_Chest.DT_Reward_Chest'");
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

	TArray<FRewardData*> datas;
	//데이터 테이블 에셋을 정상적으로 불러왔다면
	if (!!Table)
	{
		Table->GetAllRows<FRewardData>("", datas);

		for (int32 i = 0; i < datas.Num(); i++)
		{
			FRewardData* data;
			RewardDatas.Add(data);
		}
		//Write Datas from Array
		for (int32 i = 0; i < datas.Num(); i++)
		{
			for (FRewardData* data : datas)
			{
				if (datas[i]->RewardName == data->RewardName)
				{
					RewardDatas[i] = data;
					continue;
				}
			}
		}
	}

	SortRewardDatas();
}

void ACObject_Chest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaSeconds);
}

void ACObject_Chest::Begin_Interact(class ACharacter* InCharacter)
{
	//이미 상호작용 했는지 검사
	CheckTrue(bInteracted);
	bInteracted = true;
	//상호작용한 캐릭터가 플레이어인지 검사
	CheckNull(Cast<ACPlayer>(InCharacter));
	InteractedCharacter = Cast<ACPlayer>(InCharacter);

	//Interacting Mode로 바꾸기 위해 State Component를 가져온다.
	//컴포넌트가 Null값이 아니면 Interacting Mode로 바꿔준다.
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InteractedCharacter);
	CheckNull(state);
	state->SetInteractingMode();

	//캐릭터를 오브젝트 쪽으로 돌린다.
	FVector characterVector = InteractedCharacter->GetActorLocation();
	characterVector = FVector(characterVector.X, characterVector.Y, 0);
	FVector objectVector = GetActorLocation();
	objectVector = FVector(objectVector.X, objectVector.Y, 0);
	InteractedCharacter->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(characterVector, objectVector));

	//캐릭터의 HitResult를 초기화한다.
	InteractedCharacter->ResetHitResult();

	//모든 작업이 끝났으니 타임라인을 실행시킨다.
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
	//만약 상호작용을 하지 않은 상태로 진입했다면 함수를 실행시키지 않음
	CheckFalse(bInteracted);

	//Idle Mode로 바꾸기 위해 State Component를 가져온다.
	//컴포넌트가 Null값이 아니면 Idle Mode로 바꿔준다.
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InteractedCharacter);
	CheckNull(state);
	state->SetIdleMode();


	SpawnObject(GetActorLocation());
}

void ACObject_Chest::SpawnObject(FVector InLocation)
{
	FTransform transform;
	transform.SetLocation(InLocation);

	TSubclassOf<ACObject_DropItem> rewardClass;

	GetRewardClass(&rewardClass);

	CheckNull(rewardClass);

	CLog::Log(rewardClass);

	ACObject_DropItem* spawnObject =
		GetWorld()->SpawnActorDeferred<ACObject_DropItem>(
			rewardClass,
			transform
			);

	spawnObject->SetDrop(false);

	UGameplayStatics::FinishSpawningActor(spawnObject, transform);
}

void ACObject_Chest::GetRewardClass(TSubclassOf<ACObject_DropItem>* InClass)
{
	float percentage = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

	for (auto data : RewardDatas)
	{
		if (percentage <= data->Percentage)
		{
			*InClass = data->ObjectClass;
			return;
		}
	}
}

void ACObject_Chest::SortRewardDatas()
{
	RewardDatas.Sort([](const FRewardData& Data1, const FRewardData& Data2)
	{
		return Data1.Percentage < Data2.Percentage;
	}
	);
}
