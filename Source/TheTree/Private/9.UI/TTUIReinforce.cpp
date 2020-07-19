#include "TTUIReinforce.h"
#include "Components/Button.h"
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
	
	AtkUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnAtkUp);
	DefUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnDefUp);
	MaxHPUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnMaxHPUp);
	MaxStaUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnMaxStaUp);
	GetStaUpButton->OnClicked.AddDynamic(this, &UTTUIReinforce::OnGetStaUp);
}

bool UTTUIReinforce::Initialize()
{
	Super::Initialize();

	ReinforceSound = LoadObject<USoundWave>(this, TEXT("/Game/Assets/Sound/Common/Common_Reinforce_Stat.Common_Reinforce_Stat"));

	return true;
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

void UTTUIReinforce::OnAtkUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetAtk(CurrentCharacterStat->GetAtk() + 5.0f);
	UGameplayStatics::PlaySound2D(this, ReinforceSound);
}

void UTTUIReinforce::OnDefUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetDef(CurrentCharacterStat->GetDef() + 5.0f);
	UGameplayStatics::PlaySound2D(this, ReinforceSound);
}

void UTTUIReinforce::OnMaxHPUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetMaxHP(CurrentCharacterStat->GetMaxHP() + 100.0f);
	UGameplayStatics::PlaySound2D(this, ReinforceSound);
}

void UTTUIReinforce::OnMaxStaUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetMaxSta(CurrentCharacterStat->GetMaxSta() + 10.0f);
	UGameplayStatics::PlaySound2D(this, ReinforceSound);
}

void UTTUIReinforce::OnGetStaUp()
{
	TTCHECK(CurrentCharacterStat);
	CurrentCharacterStat->SetStaToGetPerHit(CurrentCharacterStat->GetStaToGetPerHit() + 1.0f);
	UGameplayStatics::PlaySound2D(this, ReinforceSound);
}
