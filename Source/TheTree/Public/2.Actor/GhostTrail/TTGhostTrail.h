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
	virtual void BeginDestroy() override;
public:	
	virtual void Tick(float DeltaTime) override;
	void SetSkeletalMesh(class USkeletalMeshComponent* Target);
	void SetMaterial(const TCHAR* Direction);
	void SetMaterial(UMaterialInterface* Material);
	void StartTrail();
	void StopTimeline();

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

	void SetGhostTrail(USkeletalMeshComponent* Component, float Interval, float Length);
	void SetMaterial(const TCHAR* Direction);
	void SetMaterial(UMaterialInterface* Material);
	void DoWork();
	void StopTrail();


private:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY()
	UMaterialInterface* TrailMaterial {nullptr};

	FTimerHandle TimerHandle;
	float LoopInterval;
	float CurrentLength;
};

void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath);
void PlayGhostTrail(USkeletalMeshComponent* Component, const TCHAR* MaterialPath, float Interval, float Length);
void PlayGhostTrail(USkeletalMeshComponent* Component, UMaterialInterface* Material, float Interval, float Length);
void PlayGhostTrail(USkeletalMeshComponent* Component, float Interval, float Length);
void StopGhostTrail(USkeletalMeshComponent* Component);


