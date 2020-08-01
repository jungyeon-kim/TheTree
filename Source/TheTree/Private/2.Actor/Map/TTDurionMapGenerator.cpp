#include "TTDurionMapGenerator.h"
#include "TTArcdevaArcher.h"
#include "TTArcdevaLancer.h"
#include "TTArcdevaWarrior.h"
#include "TTDurionMapTile.h"
#include "TTGameInstance.h"

void ATTDurionMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<bool> Map{ MakeMapTexture(20) };
	SetMapTileActorClass(ATTDurionMapTile::StaticClass());
	BuildObjects(Map, true);
	SpawnMonsters<ATTArcdevaArcher, ATTArcdevaLancer, ATTArcdevaWarrior>(TTGameInstance->GetClearCount() + 3);
}