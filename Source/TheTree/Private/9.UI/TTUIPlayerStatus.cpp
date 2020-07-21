#include "TTUIPlayerStatus.h"
#include "Components/TextBlock.h"
#include "TTCharacterStatComponent.h"

void UTTUIPlayerStatus::NativeConstruct()
{
	Super::NativeConstruct();

	HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHP")));
	StaText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtSta")));
	AtkText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtAtk")));
	DefText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDef")));
	GetStaText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtGetSta")));
	MyGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtMyGold")));
}

void UTTUIPlayerStatus::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HPText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetHP()) + " / " + 
		FString::FromInt(CurrentCharacterStat->GetMaxHP())));
	StaText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetSta()) + " / " +
		FString::FromInt(CurrentCharacterStat->GetMaxSta())));
	AtkText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetAtk())));
	DefText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetDef())));
	GetStaText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetStaToGetPerHit())));
	MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
}

void UTTUIPlayerStatus::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}
