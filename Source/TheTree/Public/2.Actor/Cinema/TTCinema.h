#pragma once

#include "TheTree.h"
#include "LevelSequenceActor.h"
#include "GameFramework/Actor.h"
#include "TTCinema.generated.h"

UCLASS()
class THETREE_API ATTCinema : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTCinema();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	virtual void Tick(float DeltaTime) override;
	void SetCinema(const TCHAR* Path);
	void PlayCinema();
	void SetAndPlayCinema(const TCHAR* Path);

	UFUNCTION()
	void StartCinemaFunction();
	UFUNCTION()
	void EndCinemaFunction();
private:
	UPROPERTY()
	class ULevelSequence* LevelSequence;

	UPROPERTY()
	class ULevelSequencePlayer* SequencePlayer;
};
