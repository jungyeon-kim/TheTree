#pragma once

#include "10.Level/TTBaseLevel.h"
#include "TTLobbyLevel.generated.h"

UCLASS()
class THETREE_API ATTLobbyLevel : public ATTBaseLevel
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ATTLobbyLevel();
	
};
