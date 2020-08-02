#pragma once

#include "TheTree.h"
#include "2.Actor/Map/TTBaseMapGenerator.h"
#include "TTTrooperMapGenerator.generated.h"

UCLASS()
class THETREE_API ATTTrooperMapGenerator : public ATTBaseMapGenerator
{
	GENERATED_BODY()
	
public:
	ATTTrooperMapGenerator() = default;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
};
