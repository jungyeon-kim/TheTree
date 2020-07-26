#include "TTTrooperBattleLevel.h"
#include "TTGameInstance.h"
#include "TTPortal.h"

void ATTTrooperBattleLevel::AddMonsterCount(int32 Count)
{
	Super::AddMonsterCount(Count);
	if (CurrentMonsterCount <= 0)
		GetWorld()->SpawnActor<ATTPortal>(ATTPortal::StaticClass(), FVector{ 0.0f, 0.0f, 130.0f }, FRotator{ 0.0f, 0.0f, 0.0f });
}