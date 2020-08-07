#include "TTUIPlayerInGame.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TTBaseLevel.h"
#include "TTCharacterStatComponent.h"
#include "TTAIStatComponent.h"
#include "TTPlayer.h"

void UTTUIPlayerInGame::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	StaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbSta")));
	BossHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbBossHP")));

	PortalText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtNextStage")));

	BossHPBarFrame = Cast<UImage>(GetWidgetFromName(TEXT("BossHPFrame")));
	SkillIcon.Emplace(Cast<UImage>(GetWidgetFromName(TEXT("Icon_Smash"))));
	SkillIcon.Emplace(Cast<UImage>(GetWidgetFromName(TEXT("Icon_SlidingSlash"))));
	SkillIcon.Emplace(Cast<UImage>(GetWidgetFromName(TEXT("Icon_WindCutter"))));
	SkillIcon.Emplace(Cast<UImage>(GetWidgetFromName(TEXT("Icon_GaiaCrush"))));
	SkillIcon.Emplace(Cast<UImage>(GetWidgetFromName(TEXT("Icon_DrawSword"))));
	for (int i = 0; i < SkillIcon.Num(); ++i) bIsSkillEnabled.Emplace(true);

	BossHPBar->SetVisibility(ESlateVisibility::Hidden);
	BossHPBarFrame->SetVisibility(ESlateVisibility::Hidden);
	PortalText->SetVisibility(ESlateVisibility::Hidden);
	TTPlayer = Cast<ATTPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	SkillCost = TTPlayer->GetSkillCost();
	
	Cast<ATTBaseLevel>(TTPlayer->GetWorld()->GetLevelScriptActor())->ClearMonsterDelegate.AddUObject(this, &UTTUIPlayerInGame::ShowPortalText);
}

void UTTUIPlayerInGame::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	HPBar->SetPercent(FMath::FInterpTo(HPBar->Percent, CurrentCharacterStat->GetHPRatio(), InDeltaTime, 5.0f));
	StaBar->SetPercent(FMath::FInterpTo(StaBar->Percent, CurrentCharacterStat->GetStaRatio(), InDeltaTime, 5.0f));
	if (CurrentAIStat) BossHPBar->SetPercent(FMath::FInterpTo(BossHPBar->Percent, CurrentAIStat->GetHPRatio(), InDeltaTime, 5.0f));

	ChangeSkillIconColor();
}

void UTTUIPlayerInGame::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	StaBar->SetPercent(CurrentCharacterStat->GetStaRatio());
}

void UTTUIPlayerInGame::BindAIStat(UTTAIStatComponent* AIStat)
{
	TTCHECK(AIStat);
	CurrentAIStat = AIStat;

	BossHPBar->SetPercent(CurrentAIStat->GetHPRatio());
}

void UTTUIPlayerInGame::ChangeSkillIconColor()
{
	for (int i = 0; i < SkillIcon.Num(); ++i)
	{
		if (bIsSkillEnabled[i] && CurrentCharacterStat->GetSta() < SkillCost[i])
		{
			SkillIcon[i]->SetColorAndOpacity(FLinearColor{ 0.2f, 0.2f, 0.2f, 0.5f });
			bIsSkillEnabled[i] = false;
		}
		else if (!bIsSkillEnabled[i] && CurrentCharacterStat->GetSta() >= SkillCost[i])
		{
			SkillIcon[i]->SetColorAndOpacity(FLinearColor{ 1.0f, 1.0f, 1.0f, 1.0f });
			bIsSkillEnabled[i] = true;
		}
	}
}

void UTTUIPlayerInGame::ShowBossHPBar(ESlateVisibility NewVisibility)
{
	BossHPBar->SetVisibility(NewVisibility);
	BossHPBarFrame->SetVisibility(NewVisibility);
}

void UTTUIPlayerInGame::ShowPortalText()
{
	PortalText->SetVisibility(ESlateVisibility::Visible);
	if (GetWidgetTreeOwningClass()->Animations.Num())
	{
		UWidgetAnimation* const AnimWidget{ GetWidgetTreeOwningClass()->Animations[0] };
		if (AnimWidget)
			PlayAnimation(AnimWidget, 0.0, INFINITY);
	}
}