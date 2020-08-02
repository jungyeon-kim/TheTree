#include "TTCommonBattleLevel.h"
#include "TTGameInstance.h"
#include "TTPortal.h"

ATTCommonBattleLevel::ATTCommonBattleLevel()
{
	ClearMonsterDelegate.AddUObject(this, &ATTCommonBattleLevel::ClearEvents);
}

void ATTCommonBattleLevel::ClearEvents()
{
	TArray<AActor*> Arr{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTPortal::StaticClass(), Arr);
	UTTGameInstance* Inst{ GetGameInstance<UTTGameInstance>() };

	if (Inst)
		Inst->SetClearCount(Inst->GetClearCount() + 1);

	if (Arr.Num())
	{
		Arr[0]->SetActorHiddenInGame(false);
		Arr[0]->SetActorEnableCollision(true);
	}
}