#include "TTBaseLevel.h"
#include "TTCinema.h"
#include "Kismet/GameplayStatics.h"

ULevelSequencePlayer* ATTBaseLevel::PlayCinematic(ULevelSequence* Sequence, bool bRunAIFlag, FName OpenLevelName)
{
	ATTCinema* Cinema{ GetWorld()->SpawnActor<ATTCinema>(ATTCinema::StaticClass()) };
	Cinema->SetAndPlayCinema(Sequence, bRunAIFlag, OpenLevelName);
	return Cinema->GetSequencePlayer();
}

void ATTBaseLevel::DestroyActors(UPARAM(ref) TArray<AActor*>& Actors)
{
	for (auto& Actor : Actors)
		Actor->Destroy();
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
