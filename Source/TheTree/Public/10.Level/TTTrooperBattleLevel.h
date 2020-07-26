#pragma once

#include "10.Level/TTBaseLevel.h"
#include "TTTrooperBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTTrooperBattleLevel : public ATTBaseLevel
{
	GENERATED_BODY()
	
public:

	virtual void AddMonsterCount(int32 Count) override;
};
