#include "TTUIRecovery.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TTCharacterStatComponent.h"
#include "TTAudioComponent.h"

void UTTUIRecovery::NativeConstruct()
{
	Super::NativeConstruct();

	RecoveryHPButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRecoveryHP")));
	RecoveryStaButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRecoverySta")));

	RecoveryHPGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtRecoveryHPGold")));
	RecoveryHPGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtRecoveryStaGold")));
	MyGoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtMyGold")));

	RecoveryHPButton->OnClicked.AddDynamic(this, &UTTUIRecovery::OnRecoveryHP);
	RecoveryStaButton->OnClicked.AddDynamic(this, &UTTUIRecovery::OnRecoverySta);

	for (int i = 0; i < 2; ++i) Price.Emplace(200);
	RecoveryHPGoldText->SetText(FText::FromString(FString::FromInt(Price[0])));
	RecoveryHPGoldText->SetText(FText::FromString(FString::FromInt(Price[1])));
}

void UTTUIRecovery::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTTUIRecovery::Init()
{
	Audio = NewObject<UTTAudioComponent>(GetOwningPlayer());
	Audio->RegisterComponent();

	Audio->AddSoundWave(TEXT("Success"), TEXT("/Game/Assets/Sound/Common/Common_Success.Common_Success"));
	Audio->AddSoundWave(TEXT("Fail"), TEXT("/Game/Assets/Sound/Common/Common_Fail.Common_Fail"));
}

void UTTUIRecovery::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}

void UTTUIRecovery::UpdatePlayerGold()
{
	MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
}

void UTTUIRecovery::OnRecoveryHP()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[0] && CurrentCharacterStat->GetHP() != CurrentCharacterStat->GetMaxHP())
	{
		CurrentCharacterStat->SetHP(CurrentCharacterStat->GetMaxHP());
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[0]);
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIRecovery::OnRecoverySta()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetGold() >= Price[1] && CurrentCharacterStat->GetSta() != CurrentCharacterStat->GetMaxSta())
	{
		CurrentCharacterStat->SetSta(CurrentCharacterStat->GetMaxSta());
		CurrentCharacterStat->SetGold(CurrentCharacterStat->GetGold() - Price[1]);
		MyGoldText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGold())));
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}
