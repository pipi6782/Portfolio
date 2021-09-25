#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "Components/TimelineComponent.h"
#include "CDoAction_Boomerang.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoomerang_End);

UCLASS()
class PORTFOLIO_API ACDoAction_Boomerang : public ACDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		float timer = 0.001f;
public:
	ACDoAction_Boomerang();

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class AActor* InOtherActor) override;

	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class AActor* InOtherActor) override;
private:
	UFUNCTION()
		void ReturnToPlayer();

	UFUNCTION()
		void OnBoomerangEnd();

	UFUNCTION()
		void OnBoomerangThrowing(float Output);

	UFUNCTION()
		void Unequip_Boomerang();

	void Abort();

private:
	class ACAttachment* boomerang;
	class UCSplineComponent* Spline;

	FTimeline Timeline;
	
	FBoomerang_End OnBoomerang_End;

	TArray<class AActor*> DamagedActors;

	class ACBossManager* bossManager;

	class ACHUD* Hud;
};
