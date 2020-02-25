#include "TTBasicEnemy.h"

ATTBasicEnemy::ATTBasicEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATTBasicEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ATTBasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
