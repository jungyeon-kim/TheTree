#pragma once

#include "TheTree.h"
#include "Engine/LevelScriptActor.h"
#include "TTPerfectDurionBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTPerfectDurionBattleLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	class ULevelSequencePlayer* PlayCinematic(class ULevelSequence* Sequence);
};
