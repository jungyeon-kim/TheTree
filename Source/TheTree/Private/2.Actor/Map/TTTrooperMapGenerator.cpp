#include "TTTrooperMapGenerator.h"
#include "TTArgoniteGiant.h"
#include "TTArgoniteGuardian.h"
#include "TTArgoniteTrooper.h"
#include "TTTrooperMapTile.h"
#include "TTDurionMineral.h"
#include "TTDurionMineralMiddle.h"
#include "TTDurionMineralLarge.h"
#include "TTGameInstance.h"

ATTTrooperMapGenerator::ATTTrooperMapGenerator()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TLDTABLE{ TEXT("/Game/GameData/TTTrooperLevelDesign.TTTrooperLevelDesign") };
	if (DT_TLDTABLE.Succeeded()) LevelDesignTable = DT_TLDTABLE.Object;
}

void ATTTrooperMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<ETextureType> Map{ MakeMapTexture(20) };
	SetMapTileActorClass(ATTTrooperMapTile::StaticClass());
	BuildObjects(Map, false);

	TArray<FActorDistElement> Dist{ {ATTDurionMineral::StaticClass(), 33.3f},
	{ATTDurionMineralMiddle::StaticClass(), 33.3f}, {ATTDurionMineralLarge::StaticClass(), 33.4f} };

	InPlaceActorRandom(Dist, 10, 100.0f);
}

void ATTTrooperMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (LevelDesignTable)
		SpawnMonsters(LevelDesignTable, TTGameInstance->GetClearCount());
	else
	{
		const float ElementPerAdd{ 5.0f * TTGameInstance->GetClearCount() };
		TArray<FActorDistElement> Dist{ {ATTArgoniteGuardian::StaticClass(), -5.0f + ElementPerAdd},
		{ATTArgoniteGiant::StaticClass(), ElementPerAdd} ,{ATTArgoniteTrooper::StaticClass(), 100.0f} };
		Dist.Sort([](const FActorDistElement& lhs, const FActorDistElement& rhs) {return lhs.Percentage < rhs.Percentage; });

		SpawnMonsters(Dist, TTGameInstance->GetClearCount() + 2);
	}
}
