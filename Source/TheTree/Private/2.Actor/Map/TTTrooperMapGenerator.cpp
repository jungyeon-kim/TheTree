#include "TTTrooperMapGenerator.h"
#include "TTArgoniteGiant.h"
#include "TTArgoniteGuardian.h"
#include "TTArgoniteTrooper.h"
#include "TTTrooperMapTile.h"
#include "TTGameInstance.h"

ATTTrooperMapGenerator::ATTTrooperMapGenerator()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TLDTABLE{ TEXT("/Game/GameData/TTTrooperLevelDesign.TTTrooperLevelDesign") };
	if (DT_TLDTABLE.Succeeded()) LevelDesignTable = DT_TLDTABLE.Object;
}

void ATTTrooperMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<bool> Map{ MakeMapTexture(20) };
	SetMapTileActorClass(ATTTrooperMapTile::StaticClass());
	BuildObjects(Map, false);
}

void ATTTrooperMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (LevelDesignTable)
		SpawnMonsters(LevelDesignTable, TTGameInstance->GetClearCount());
	else
	{
		const float ElementPerAdd{ 5.0f * TTGameInstance->GetClearCount() };
		TArray<FMonsterDistElement> Dist{ {ATTArgoniteGuardian::StaticClass(), -5.0f + ElementPerAdd},
		{ATTArgoniteGiant::StaticClass(), ElementPerAdd} ,{ATTArgoniteTrooper::StaticClass(), 100.0f} };
		Dist.Sort([](const FMonsterDistElement& lhs, const FMonsterDistElement& rhs) {return lhs.Percentage < rhs.Percentage; });

		SpawnMonsters(Dist, TTGameInstance->GetClearCount() + 2);
	}
}
