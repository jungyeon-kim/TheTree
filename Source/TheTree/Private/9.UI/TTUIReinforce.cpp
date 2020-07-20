#include "TTUIReinforce.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TTCharacterStatComponent.h"
#include "TTAudioComponent.h"

void UTTUIReinforce::NativeConstruct()
{
	Super::NativeConstruct();

	AtkUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnAtkUp")));
	DefUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnDefUp")));
	MaxHPUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnMaxHPUp")));
	MaxStaUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnMaxStaUp")));
	GetStaUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnGetStaUp")));

	AtkGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtAtkGold")));
	DefGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDefGold")));
	MaxHPGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtMaxHPGold")));
	MaxStaGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtMaxStaGold")));
	GetStaGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtGetStaGold")));
	MyGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtMyGold")));
	
	AtkUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnAtkUp);
	DefUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnDefUp);
	MaxHPUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnMaxHPUp);
	MaxStaUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnMaxStaUp);
	GetStaUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnGetStaUp);

	for (int i = 0; i < 5; ++i) Price.Emplace(100);
	AtkGoldText->SetText(FText::FromString(FString::FromInt(Price[0])));
	DefGoldText->SetText(FText::FromString(FString::FromInt(Price[1])));
	MaxHPGoldText->SetText(FText::FromString(FString::FromInt(Price[2])));
	MaxStaGoldText->SetText(FText::FromString(FString::FromInt(Price[3])));
	GetStaGoldText->SetText(FText::FromString(FString::FromInt(Price[4])));
}

void UTTUIReinforce::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTTUIReinforce::Init()
{
	Audio = NewObject<UTTAudioComponent>(GetOwningPlayer());
	Audio->RegisterComponent();

	Audio->AddSoundWave(TEXT("Success"), TEXT("/Game/Assets/Sound/Common/Common_Success.Common_Success"));
	Audio->AddSoundWave(TEXT("Fail"), TEXT("/Game/Assets/Sound/Common/Common_Fail.Common_Fail"));
}

void UTTUIReinforce::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}

void UTTUIReinforce::UpdatePlayerGold()
{
	MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
}

void UTTUIReinforce::OnAtkUp()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[0])
	{
		CurrentCharacterStat->SetAtk(CurrentCharacterStat->GetAtk() + 5.0f);
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[0]);
		AtkGoldText->SetText(FText::FromString(FString::FromInt(Price[0] += 100)));
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIReinforce::OnDefUp()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[1])
	{
		CurrentCharacterStat->SetDef(CurrentCharacterStat->GetDef() + 5.0f);
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[1]);
		DefGoldText->SetText(FText::FromString(FString::FromInt(Price[1] += 100)));
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIReinforce::OnMaxHPUp()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[2])
	{
		CurrentCharacterStat->SetMaxHP(CurrentCharacterStat->GetMaxHP() + 100.0f);
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[2]);
		MaxHPGoldText->SetText(FText::FromString(FString::FromInt(Price[2] += 100)));
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIReinforce::OnMaxStaUp()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[3])
	{
		CurrentCharacterStat->SetMaxSta(CurrentCharacterStat->GetMaxSta() + 10.0f);
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[3]);
		MaxStaGoldText->SetText(FText::FromString(FString::FromInt(Price[3] += 100)));
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIReinforce::OnGetStaUp()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[4])
	{
		CurrentCharacterStat->SetStaToGetPerHit(CurrentCharacterStat->GetStaToGetPerHit() + 1.0f);
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[4]);
		GetStaGoldText->SetText(FText::FromString(FString::FromInt(Price[4] += 100)));
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}
