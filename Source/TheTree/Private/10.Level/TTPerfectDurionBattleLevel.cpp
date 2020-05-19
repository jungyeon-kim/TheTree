#include "TTPerfectDurionBattleLevel.h"
#include "LevelSequence.h"
#include "TTCinema.h"

ULevelSequencePlayer* ATTPerfectDurionBattleLevel::PlayCinematic(class ULevelSequence* Sequence)
{
	ATTCinema* Cinema{ GetWorld()->SpawnActor<ATTCinema>(ATTCinema::StaticClass()) };
	Cinema->SetAndPlayCinema(Sequence);
	return Cinema->GetSequencePlayer();
}