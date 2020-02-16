#pragma once

#include "TheTree.h"
#include "GameFramework/GameModeBase.h"
#include "TTGameMode.generated.h"

UCLASS()
class THETREE_API ATTGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	ATTGameMode();
public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
