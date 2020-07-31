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

	RecoveryHPButton->OnClicked.AddDynamic(this, &UTTUIRecovery::OnRecoveryHP);
	RecoveryStaButton->OnClicked.AddDynamic(this, &UTTUIRecovery::OnRecoverySta);
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

void UTTUIRecovery::OnRecoveryHP()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetHP() != CurrentCharacterStat->GetMaxHP())
	{
		CurrentCharacterStat->SetHP(CurrentCharacterStat->GetMaxHP());
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}

void UTTUIRecovery::OnRecoverySta()
{
	TTCHECK(CurrentCharacterStat);

	if (CurrentCharacterStat->GetSta() != CurrentCharacterStat->GetMaxSta())
	{
		CurrentCharacterStat->SetSta(CurrentCharacterStat->GetMaxSta());
		Audio->PlaySoundWave2D(TEXT("Success"));
	}
	else Audio->PlaySoundWave2D(TEXT("Fail"));
}
