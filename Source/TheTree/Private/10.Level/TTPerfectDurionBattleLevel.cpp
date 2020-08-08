#include "TTPerfectDurionBattleLevel.h"
#include "TTCinema.h"

ATTPerfectDurionBattleLevel::ATTPerfectDurionBattleLevel()
{
	ClearMonsterDelegate.AddUObject(this, &ATTPerfectDurionBattleLevel::ClearEvents);
}

void ATTPerfectDurionBattleLevel::ClearEvents()
{
	BackGroundSound->Stop();
	ULevelSequence* Sequence
	{ LoadObject<ULevelSequence>(this, TEXT("/Game/Level/Cinema/CI_Perfect_Durion_End.CI_Perfect_Durion_End")) };
	Cast<ATTBaseLevel>(GetWorld()->GetLevelScriptActor())->PlayCinematic(Sequence, true, FName{}, true);
}

void ATTPerfectDurionBattleLevel::SpawnSoundAndBackup(USoundBase* Sound)
{
	BackGroundSound = UGameplayStatics::SpawnSound2D(GetWorld(), Sound);
}

