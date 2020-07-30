#pragma once

#include "TTBaseLevel.h"
#include "TTCommonBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTCommonBattleLevel : public ATTBaseLevel
{
	GENERATED_BODY()
public:
	ATTCommonBattleLevel();
	virtual void AddMonsterCount(int32 Count) override;
};
