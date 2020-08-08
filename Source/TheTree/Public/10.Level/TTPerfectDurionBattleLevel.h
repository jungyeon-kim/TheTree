#pragma once

#include "TTBaseLevel.h"
#include "TTPerfectDurionBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTPerfectDurionBattleLevel : public ATTBaseLevel
{
	GENERATED_BODY()

	ATTPerfectDurionBattleLevel();
private:
	UPROPERTY()
	UAudioComponent* BackGroundSound;

public:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void SpawnSoundAndBackup(USoundBase* Sound);

	void ClearEvents();
};
