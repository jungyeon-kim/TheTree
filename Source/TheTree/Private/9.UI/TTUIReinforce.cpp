#include "TTUIReinforce.h"
#include "Components/Button.h"
#include "TTCharacterStatComponent.h"

void UTTUIReinforce::NativeConstruct()
{
	Super::NativeConstruct();

	StkUpButton = Cast<UButton>(GetWidgetFromName(TEXT("btnAtkUp")));
	TTCHECK(StkUpButton);

	StkUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnStkUp);
}

void UTTUIReinforce::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTTUIReinforce::BindCharacterStat(UTTCharacterStatComponent* CharacterStat)
{
	TTCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
}

void UTTUIReinforce::OnStkUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetAtk(CurrentCharacterStat->GetAtk() + 5.0f);
}
