#include "TTDurionMapGenerator.h"
#include "TTArcdevaArcher.h"
#include "TTArcdevaLancer.h"
#include "TTArcdevaWarrior.h"
#include "TTDurionMapTile.h"
#include "TTGameInstance.h"

ATTDurionMapGenerator::ATTDurionMapGenerator()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_DLDTABLE{ TEXT("/Game/GameData/TTDurionLevelDesign.TTDurionLevelDesign") };
	if (DT_DLDTABLE.Succeeded()) LevelDesignTable = DT_DLDTABLE.Object;
}

void ATTDurionMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<ETTTextureType> Map{ MakeMapTexture(20) };
	SetMapTileActorClass(ATTDurionMapTile::StaticClass());
	BuildObjects(Map, true);
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
