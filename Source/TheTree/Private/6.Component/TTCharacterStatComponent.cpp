#include "TTCharacterStatComponent.h"
#include "TTGameInstance.h"

UTTCharacterStatComponent::UTTCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UTTCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UTTCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTTCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FName UTTCharacterStatComponent::GetObjectName() const
{
	TTCHECK(TTCharacterData, NAME_None);
	return ObjectName;
}

int32 UTTCharacterStatComponent::GetGold() const
{
	return Gold;
}

float UTTCharacterStatComponent::GetHPRatio() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return MaxHP <= 0.0f ? 0.0f : CurrentHP / MaxHP;
}

float UTTCharacterStatComponent::GetStaRatio() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return MaxSta <= 0.0f ? 0.0f : CurrentSta / MaxSta;
}

float UTTCharacterStatComponent::GetMaxHP() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return MaxHP;
}

float UTTCharacterStatComponent::GetHP() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return CurrentHP;
}

float UTTCharacterStatComponent::GetMaxSta() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return MaxSta;
}

float UTTCharacterStatComponent::GetSta() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return CurrentSta;
}

float UTTCharacterStatComponent::GetStaToGetPerHit() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return StaToGetPerHit;
}

float UTTCharacterStatComponent::GetAtk() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return Atk;
}

float UTTCharacterStatComponent::GetDef() const
{
	TTCHECK(TTCharacterData, 0.0f);
	return Def;
}

void UTTCharacterStatComponent::SetObjectStat(FName NewObjectName, UGameInstance* GameInst)
{
	TTCHECK(GameInst);
	const auto TTGameInstance{ Cast<UTTGameInstance>(GameInst) };
	if (TTGameInstance) TTCharacterData = TTGameInstance->GetTTCharacterData(NewObjectName);

	if (TTCharacterData)
	{
		ObjectName = NewObjectName;
		SetGold(TTCharacterData->Gold);
		SetMaxHP(TTCharacterData->MaxHP);
		SetMaxSta(TTCharacterData->MaxSta);
		SetHP(TTCharacterData->MaxHP);
		SetSta(0.0f);
		SetStaToGetPerHit(TTCharacterData->StaToGetPerHit);
		SetAtk(TTCharacterData->Atk);
		SetDef(TTCharacterData->Def);
	}
	else TTLOG(Error, TEXT("ObjectName (%s) data doesn't exist"), *NewObjectName.ToString());
}

void UTTCharacterStatComponent::SetGold(int32 NewGold)
{
	Gold = NewGold;
}

void UTTCharacterStatComponent::SetMaxHP(float NewMaxHP)
{
	MaxHP = NewMaxHP;
}

void UTTCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);
	if (!CurrentHP) OnHPIsZero.Broadcast();
}

void UTTCharacterStatComponent::SetMaxSta(float NewMaxSta)
{
	MaxSta = NewMaxSta;
}

void UTTCharacterStatComponent::SetSta(float NewSta)
{
	CurrentSta = FMath::Clamp<float>(NewSta, 0.0f, MaxSta);
}

void UTTCharacterStatComponent::SetStaToGetPerHit(float NewStaToGetPerHit)
{
	StaToGetPerHit = NewStaToGetPerHit;
}

void UTTCharacterStatComponent::SetAtk(float NewAtk)
{
	Atk = NewAtk;
	if (Atk <= 0.0f) Atk = 0.0f;
}

void UTTCharacterStatComponent::SetDef(float NewDef)
{
	Def = FMath::Clamp<float>(NewDef, 0.0f, 70.0f);
}

void UTTCharacterStatComponent::SetDamage(float NewDamage)
{
	TTCHECK(TTCharacterData);
	NewDamage *= 1.0f - Def / 100.0f;
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));
}
