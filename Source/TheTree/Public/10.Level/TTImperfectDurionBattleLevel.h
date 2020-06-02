#pragma once

#include "TTBaseLevel.h"
#include "TTImperfectDurionBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTImperfectDurionBattleLevel : public ATTBaseLevel
{
	GENERATED_BODY()

public:

	virtual void AddMonsterCount(int32 Count) override;
};
