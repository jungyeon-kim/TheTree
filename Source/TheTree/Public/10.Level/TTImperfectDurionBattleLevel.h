#pragma once

#include "TheTree.h"
#include "Engine/LevelScriptActor.h"
#include "TTImperfectDurionBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTImperfectDurionBattleLevel : public ALevelScriptActor
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	class ULevelSequencePlayer* PlayCinematic(class ULevelSequence* Sequence);
};
