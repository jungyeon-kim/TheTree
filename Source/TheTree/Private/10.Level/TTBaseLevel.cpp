#include "TTBaseLevel.h"
#include "TTCinema.h"
#include "Kismet/GameplayStatics.h"
ULevelSequencePlayer* ATTBaseLevel::PlayCinematic(ULevelSequence* Sequence, bool bRunAIFlags)
{
	ATTCinema* Cinema{ GetWorld()->SpawnActor<ATTCinema>(ATTCinema::StaticClass()) };
	Cinema->SetAndPlayCinema(Sequence, bRunAIFlags);
	return Cinema->GetSequencePlayer();
}

void ATTBaseLevel::SetMonsterCount(int32 Count)
{
	CurrentMonsterCount = Count;
}

void ATTBaseLevel::AddMonsterCount(int32 Count)
{
	CurrentMonsterCount += Count;
}

int32 ATTBaseLevel::GetMonsterCount()
{
	return CurrentMonsterCount;
}
