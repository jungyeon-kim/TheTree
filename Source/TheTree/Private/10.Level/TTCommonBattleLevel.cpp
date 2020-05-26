#include "TTCommonBattleLevel.h"
#include "TTGameInstance.h"
#include "TTPortal.h"
ATTCommonBattleLevel::ATTCommonBattleLevel()
{
	SetMonsterCount(3);
}

void ATTCommonBattleLevel::AddMonsterCount(int32 Count)
{
	Super::AddMonsterCount(Count);
	if (CurrentMonsterCount <= 0)
	{
		TArray<AActor*> Arr{};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTPortal::StaticClass(), Arr);
		UTTGameInstance* Inst{ GetGameInstance<UTTGameInstance>() };

		if (Inst)
			Inst->AddClearCount();

		if (Arr.Num())
		{
			Arr[0]->SetActorHiddenInGame(false);
			Arr[0]->SetActorEnableCollision(true);
		}
	}
}
