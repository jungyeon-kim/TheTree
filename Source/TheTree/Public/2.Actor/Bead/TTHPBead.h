#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTHPBead.generated.h"

UCLASS()
class THETREE_API ATTHPBead : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
	float SpawnedTime;
	UPROPERTY()
	float Angle;
	UPROPERTY()
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Sphere;
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UPointLightComponent* Light;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoundBox;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
protected:
	virtual void BeginPlay() override;
public:	
	ATTHPBead();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
