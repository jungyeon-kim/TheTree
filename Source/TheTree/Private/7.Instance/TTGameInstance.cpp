#include "TTGameInstance.h"
#include "TTCharacterStatComponent.h"

UTTGameInstance::UTTGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TTCHARACTER{ TEXT("/Game/GameData/TTCharacterData.TTCharacterData") };
	if (DT_TTCHARACTER.Succeeded()) TTCharacterTable = DT_TTCHARACTER.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TTAI{ TEXT("/Game/GameData/TTAIData.TTAIData") };
	if (DT_TTAI.Succeeded()) TTAITable = DT_TTAI.Object;

	TTPlayerStat = CreateDefaultSubobject<UTTCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}

void UTTGameInstance::Init()
{
	Super::Init();

	TTPlayerStat->SetObjectStat(TEXT("Player"), this);
}

FTTCharacterData* UTTGameInstance::GetTTCharacterData(FName ObjectName) const
{
	return TTCharacterTable->FindRow<FTTCharacterData>(ObjectName, TEXT(""));
}

FTTAIData* UTTGameInstance::GetTTAIData(FName ObjectName) const
{
	return TTAITable->FindRow<FTTAIData>(ObjectName, TEXT(""));
}

UTTCharacterStatComponent* UTTGameInstance::GetTTPlayerStat() const
{
	return TTPlayerStat;
}

int32 UTTGameInstance::GetClearCount()
{
	return ClearCount;
}

void UTTGameInstance::SetClearCount(int32 Count)
{
	ClearCount = Count;
}

void UTTGameInstance::SetButtonCluster(TArray<class UTTUIMapButton*>& ButtonCluster)
{
	UIMapButtonCluster = std::move(ButtonCluster);
}

TArray<class UTTUIMapButton*>& UTTGameInstance::GetButtonCluster()
{
	return UIMapButtonCluster;
}

void UTTGameInstance::SetClearTrooper(bool bCleared)
{
	bIsClearTrooper = bCleared;
}
bool UTTGameInstance::GetClearTrooper()
{
	return bIsClearTrooper;
}
