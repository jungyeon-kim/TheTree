#include "TTUIPlayerInGame.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TTCharacterStatComponent.h"

void UTTUIPlayerInGame::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	TTCHECK(HPBar);
	StaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbSta")));
	TTCHECK(StaBar);

	HPBar->SetPercent(100.0f);
	StaBar->SetPercent(0.0f);
}

void UTTUIPlayerInGame::UpdateCharacterStat()
{
	TTCHECK(CurrentCharacterStat);
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	StaBar->SetPercent(CurrentCharacterStat->GetStaRatio());
}

void UTTUIPlayerInGame::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CurrentCharacterStat->OnHPChanged.AddUObject(this, &UTTUIPlayerInGame::UpdateCharacterStat);
	CurrentCharacterStat->OnStaChanged.AddUObject(this, &UTTUIPlayerInGame::UpdateCharacterStat);
}