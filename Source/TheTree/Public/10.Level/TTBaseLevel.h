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
	int32 CurrentMonsterCount{};

public:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	ULevelSequencePlayer* PlayCinematic(ULevelSequence* Sequence, bool bRunAIFlag = true, FName OpenLevelName = "");

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void DestroyActors(UPARAM(ref) TArray<AActor*>& Actors);

	void SetMonsterCount(int32 Count);
	
	virtual void AddMonsterCount(int32 Count);
	int32 GetMonsterCount();
};
