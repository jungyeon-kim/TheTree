#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TTGhostTrail.generated.h"

UCLASS()
class THETREE_API ATTGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTGhostTrail();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetSkeletalMesh(class USkeletalMeshComponent* Target);
	void StartTrail();

	UFUNCTION()
	void PlayingTimeline(float CurrentTime);

	UFUNCTION()
	void EndTimeline();

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	class UPoseableMeshComponent* TrailMesh;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UMaterialInstanceDynamic* MatInstanceDynamic;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UMaterialInterface* TrailMaterial;

	UPROPERTY(EditAnywhere, Category = "Timeline", Meta = (AllowPrivateAccess = true))
	UCurveFloat* TrailCurve;

	FTimeline TrailTimeline{};
};

UCLASS()
class THETREE_API ATTGhostTrailLoop : public AActor
{
public:
	GENERATED_BODY()

	ATTGhostTrailLoop();

	void SetGhostTrail(USkeletalMeshComponent* Component, float Interval, float LoopLength);
	void DoWork();
private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh;

	FTimerHandle TimerHandle;
	float LoopInterval;
	float LoopLength;
};

void PlayGhostTrail(USkeletalMeshComponent* Component);
void PlayGhostTrail(USkeletalMeshComponent* Component, float Interval, float Length);
#define TT_PLAY_GHOSTTRAIL(SkeletalMeshComponent) PlayGhostTrail(SkeletalMeshComponent)
#define TT_PLAY_GHOSTTRAIL_LOOP(SkeletalMeshComponent, Interval, Length) PlayGhostTrail(SkeletalMeshComponent, Interval, Length)
	


