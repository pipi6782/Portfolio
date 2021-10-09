#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossManager.generated.h"

//보스가 합쳐지는지에 대한 여부는 무조건 부메랑이 정해진 루트를 다 돌았을때만 검사

UCLASS()
class PORTFOLIO_API ACBossManager : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		float AttackTime = 20.0f;

	UPROPERTY(EditAnywhere)
		bool bPrintTime;

public:	
	ACBossManager();

	UFUNCTION()
		void Reset();

	UFUNCTION()
		void OnBossAttackTimer();

	void StartBossAttackTimer();

	virtual void Tick(float DeltaTime) override;

	void AddBoss(class ACEnemy_Boss* InBoss);

	UFUNCTION()
		void CheckBossOnBoomerangEnd();

protected:
	virtual void BeginPlay() override;

private:

	bool IsBossOrderingCorrect();

public:	
	TArray<class ACEnemy_Boss*> Bosses, CorrectBoss;

private:
	bool bEnableTick;
	float RunningTime = 0.0f;
};
