#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTReinforceNPC.generated.h"

UCLASS()
class THETREE_API ATTReinforceNPC : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoundBox;
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
public:	
	ATTReinforceNPC();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex);
};
