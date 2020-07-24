#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTManual.generated.h"

UCLASS()
class THETREE_API ATTManual : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Plain;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoundBox;

	UPROPERTY()
	class ATTPlayerController* TTPlayerController;
protected:
	virtual void BeginPlay() override;
public:	
	ATTManual();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex);
};
