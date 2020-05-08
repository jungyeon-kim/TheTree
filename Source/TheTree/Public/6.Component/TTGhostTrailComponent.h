#pragma once

#include "TheTree.h"
#include "TTGhostTrailComponent.generated.h"

UCLASS()
class THETREE_API UTTGhostTrailComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	USkeletalMeshComponent* Pose;

	float LoopInterval;
	float CurrentLength;
	FTimerHandle TimerHandle;

	void DoWork();
public:
	UTTGhostTrailComponent() = default;
	void SetGhostTrail(const TCHAR* MaterialPath, USkeletalMeshComponent* TargetPose);
	void PlayGhostTrail(float Interval, float Length);
	void StopGhostTrail();
};
