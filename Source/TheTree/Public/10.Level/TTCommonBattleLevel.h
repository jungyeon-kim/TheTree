#pragma once

#include "TTBaseLevel.h"
#include "TTCommonBattleLevel.generated.h"

UCLASS()
class THETREE_API ATTCommonBattleLevel : public ATTBaseLevel
{
	GENERATED_BODY()

private:
	void ClearEvents();

public:
	ATTCommonBattleLevel();
};
