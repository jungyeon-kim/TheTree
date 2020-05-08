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
	void SetGhostTrail(USkeletalMeshComponent* Pose, UMaterialInterface* Mat);
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
	class UMaterialInstanceDynamic* MatInstanceDynamic;

	UPROPERTY(EditAnywhere, Category = "Timeline", Meta = (AllowPrivateAccess = true))
	UCurveFloat* TrailCurve;

	FTimeline TrailTimeline{};
};