#include "TTBaseLevel.h"
#include "TTCinema.h"

ULevelSequencePlayer* ATTBaseLevel::PlayCinematic(ULevelSequence* Sequence)
{
	ATTCinema* Cinema{ GetWorld()->SpawnActor<ATTCinema>(ATTCinema::StaticClass()) };
	Cinema->SetAndPlayCinema(Sequence);
	return Cinema->GetSequencePlayer();
}