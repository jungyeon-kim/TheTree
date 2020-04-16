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
	//TArray<UMaterialInstanceDynamic*> 
};

#define TT_PLAY_GHOSTTRAIL(SkeletalMeshComponent) { FActorSpawnParameters Param{}; Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; ATTGhostTrail* Trail{ GetWorld()->SpawnActor<ATTGhostTrail>(ATTGhostTrail::StaticClass(), SkeletalMeshComponent->GetComponentTransform(), Param)}; Trail->SetSkeletalMesh(SkeletalMeshComponent); Trail->StartTrail(); }
	


