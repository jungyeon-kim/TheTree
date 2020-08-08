#include "TTPerfectDurionBattleLevel.h"
#include "TTCinema.h"
#include "TTPortal.h"

ATTPerfectDurionBattleLevel::ATTPerfectDurionBattleLevel()
{
	ClearMonsterDelegate.AddUObject(this, &ATTPerfectDurionBattleLevel::ClearEvents);
}

void ATTPerfectDurionBattleLevel::ClearEvents()
{
	BackGroundSound->Stop();
	ULevelSequence* Sequence
	{ LoadObject<ULevelSequence>(this, TEXT("/Game/Level/Cinema/CI_Perfect_Durion_End.CI_Perfect_Durion_End")) };
	ULevelSequencePlayer* SequencePlayer{ Cast<ATTBaseLevel>(GetWorld()->GetLevelScriptActor())->PlayCinematic(Sequence, true, FName{}, true) };

	FScriptDelegate EndFuncDelegate{};
	EndFuncDelegate.BindUFunction(this, "SpawnPortal");
	SequencePlayer->OnFinished.Add(EndFuncDelegate);
}

void ATTPerfectDurionBattleLevel::SpawnSoundAndBackup(USoundBase* Sound)
{
	BackGroundSound = UGameplayStatics::SpawnSound2D(GetWorld(), Sound);
}

void ATTPerfectDurionBattleLevel::SpawnPortal()
{
	ATTPortal* Portal{ GetWorld()->SpawnActor<ATTPortal>(ATTPortal::StaticClass(), FVector{ 0.0f, 1800.0f, 130.0f }, FRotator::ZeroRotator) };
	Portal->SetActorHiddenInGame(false);
	Portal->SetActorEnableCollision(true);
}
