#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTPortal.generated.h"

UCLASS()
class THETREE_API ATTPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTPortal();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UParticleSystemComponent* PortalParticle;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* OverlapBox;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
