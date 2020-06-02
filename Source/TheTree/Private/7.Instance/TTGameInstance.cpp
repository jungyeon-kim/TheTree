#include "TTGameInstance.h"
#include "TTCharacterStatComponent.h"

UTTGameInstance::UTTGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TTCHARACTER{ TEXT("/Game/GameData/TTCharacterData.TTCharacterData") };
	if (DT_TTCHARACTER.Succeeded()) TTCharacterTable = DT_TTCHARACTER.Object;

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

UTTCharacterStatComponent* UTTGameInstance::GetTTPlayerStat() const
{
	return TTPlayerStat;
}

int32 UTTGameInstance::GetClearCount()
{
	return ClearCount;
}

void UTTGameInstance::AddClearCount()
{
	++ClearCount;
}

const int32 UTTGameInstance::GetPlanOfClearCount()
{
	return PlanOfClearCount;
}
