#include "TTUIPlayerStatus.h"
#include "TTCharacterStatComponent.h"

void UTTUIPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTTUIPlayerStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTTUIPlayerStatus::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}
