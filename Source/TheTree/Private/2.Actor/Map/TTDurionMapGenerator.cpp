#include "TTDurionMapGenerator.h"
#include "TTArcdevaArcher.h"
#include "TTArcdevaLancer.h"
#include "TTArcdevaWarrior.h"
#include "TTDurionMapTile.h"
#include "TTDurionMineral.h"
#include "TTDurionMineralMiddle.h"
#include "TTDurionMineralLarge.h"
#include "TTGameInstance.h"

ATTDurionMapGenerator::ATTDurionMapGenerator()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DLDTABLE{ TEXT("/Game/GameData/TTDurionLevelDesign.TTDurionLevelDesign") };
	if (DT_DLDTABLE.Succeeded()) LevelDesignTable = DT_DLDTABLE.Object;
}

void ATTDurionMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<ETextureType> Map{ MakeMapTexture(20) };
	SetMapTileActorClass(ATTDurionMapTile::StaticClass());
	BuildObjects(Map, true);

	TArray<FActorDistElement> Dist{ {ATTDurionMineral::StaticClass(), 33.3f},
		{ATTDurionMineralMiddle::StaticClass(), 33.3f}, {ATTDurionMineralLarge::StaticClass(), 33.4f} };

	InPlaceActorRandom(Dist, 10, 100.0f);
}

void ATTDurionMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (LevelDesignTable)
		SpawnMonsters(LevelDesignTable, TTGameInstance->GetClearCount());
	else
	{
		const float ElementPerAdd{ 5.0f * TTGameInstance->GetClearCount() };
		TArray<FActorDistElement> Dist{ {ATTArcdevaLancer::StaticClass(), -5.0f + ElementPerAdd},
			{ATTArcdevaArcher::StaticClass(), ElementPerAdd} ,{ATTArcdevaWarrior::StaticClass(), 105.0f - ElementPerAdd} };
		Dist.Sort([](const FActorDistElement& lhs, const FActorDistElement& rhs) {return lhs.Percentage < rhs.Percentage; });

		SpawnMonsters(Dist, TTGameInstance->GetClearCount() + 3);
	}
}
