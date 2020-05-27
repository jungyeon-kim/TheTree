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

void UTTUIPlayerInGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HPBar->SetPercent(FMath::FInterpTo(HPBar->Percent, CurrentCharacterStat->GetHPRatio(), InDeltaTime, 5.0f));
	StaBar->SetPercent(FMath::FInterpTo(StaBar->Percent, CurrentCharacterStat->GetStaRatio(), InDeltaTime, 5.0f));
}

void UTTUIPlayerInGame::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}