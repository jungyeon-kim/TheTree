#include "TTDurionMapGenerator.h"

#include "TTArcdevaArcher.h"
#include "TTArcdevaLancer.h"
#include "TTArcdevaWarrior.h"

void ATTDurionMapGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TArray<bool> Map{ MakeMapTexture() };
	CelluarAutomata(Map, 20);
	FinalWork(Map);

	BuildObjects(Map);
	SetMonsters<ATTArcdevaArcher, ATTArcdevaWarrior, ATTArcdevaLancer>();
}