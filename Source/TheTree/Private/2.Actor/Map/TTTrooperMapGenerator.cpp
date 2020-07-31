#include "TTTrooperMapGenerator.h"
#include "TTArgoniteGiant.h"
#include "TTArgoniteGuardian.h"
#include "TTArgoniteTrooper.h"
#include "TTTrooperMapTile.h"
#include "TTGameInstance.h"

void ATTTrooperMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<bool> Map{ MakeMapTexture() };
	CelluarAutomata(Map, 20);
	FinalWork(Map);

	SetMapTileActorClass(ATTTrooperMapTile::StaticClass());

	BuildObjects(Map, false);

	SpawnMonsters<ATTArgoniteGiant, ATTArgoniteGuardian, ATTArgoniteTrooper>(TTGameInstance->GetClearCount()+2);

	TurnToMonster();
}
