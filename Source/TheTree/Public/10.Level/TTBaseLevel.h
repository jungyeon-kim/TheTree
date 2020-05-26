#pragma once

#include "TheTree.h"
#include "Engine/LevelScriptActor.h"
#include "LevelSequencePlayer.h"
#include "TTBaseLevel.generated.h"

UCLASS()
class THETREE_API ATTBaseLevel : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	ULevelSequencePlayer* PlayCinematic(ULevelSequence* Sequence);
};
