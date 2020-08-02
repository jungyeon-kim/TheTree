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
}

void ATTDurionMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	const float ElementPerAdd{ 5.0f * TTGameInstance->GetClearCount() };
	TArray<FMonsterDistElement> Dist{ {ATTArcdevaLancer::StaticClass(), -5.0f + ElementPerAdd},
		{ATTArcdevaArcher::StaticClass(), ElementPerAdd} ,{ATTArcdevaWarrior::StaticClass(), 105.0f - ElementPerAdd} };
	Dist.Sort([](const FMonsterDistElement& lhs, const FMonsterDistElement& rhs) {return lhs.Percentage < rhs.Percentage; });

	SpawnMonsters(Dist, TTGameInstance->GetClearCount() + 3);
}
