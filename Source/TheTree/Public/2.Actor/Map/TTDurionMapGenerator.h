#pragma once

#include "TheTree.h"
#include "2.Actor/Map/TTBaseMapGenerator.h"
#include "TTDurionMapGenerator.generated.h"

UCLASS()
class THETREE_API ATTDurionMapGenerator : public ATTBaseMapGenerator
{
	GENERATED_BODY()

public:
	ATTDurionMapGenerator() = default;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
};
