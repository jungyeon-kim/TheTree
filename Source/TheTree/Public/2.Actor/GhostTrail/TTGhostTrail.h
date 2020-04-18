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
	void SetMaterial(const TCHAR* Direction);
	void SetMaterial(UMaterialInterface* Material);
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
	void SetMaterial(const TCHAR* Direction);
	void DoWork();

private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	UMaterialInterface* TrailMaterial {nullptr};

	FTimerHandle TimerHandle;
	float LoopInterval;
	float LoopLength;
};

void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath);
void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath, float Interval, float Length);
void StopGhostTrail(USkeletalMeshComponent* Component);
#define TT_PLAY_GHOSTTRAIL(SkeletalMeshComponent, MaterialPath) PlayGhostTrail(SkeletalMeshComponent, MaterialPath)
#define TT_PLAY_GHOSTTRAIL_LOOP(SkeletalMeshComponent, MaterialPath, Interval, Length) PlayGhostTrail(SkeletalMeshComponent, MaterialPath, Interval, Length)
#define TT_STOP_GHOSTTRAIL_LOOP(SkeletalMeshComponent) StopGhostTrail(SkeletalMeshComponent)


