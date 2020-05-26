#include "TTGameInstance.h"

UTTGameInstance::UTTGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TTCHARACTER{ TEXT("/Game/GameData/TTCharacterData.TTCharacterData") };
	if (DT_TTCHARACTER.Succeeded()) TTCharacterTable = DT_TTCHARACTER.Object;
}

void UTTGameInstance::Init()
{
	Super::Init();
}

FTTCharacterData* UTTGameInstance::GetTTCharacterData(FName ObjectName)
{
	return TTCharacterTable->FindRow<FTTCharacterData>(ObjectName, TEXT(""));
}

int UTTGameInstance::GetClearCount()
{
	return ClearCount;
}

void UTTGameInstance::AddClearCount()
{
	++ClearCount;
}

const int UTTGameInstance::GetPlanOfClearCount()
{
	return PlanOfClearCount;
}
