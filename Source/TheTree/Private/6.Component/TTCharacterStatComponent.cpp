#include "TTCharacterStatComponent.h"
#include "TTGameInstance.h"

UTTCharacterStatComponent::UTTCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
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
	TTCHECK(CurrentStatData, NAME_None);
	return ObjectName;
}

float UTTCharacterStatComponent::GetHPRatio() const
{
	TTCHECK(CurrentStatData, 0.0f);
	return CurrentStatData->MaxHP <= 0.0f ? 0.0f : CurrentHP / CurrentStatData->MaxHP;
}

float UTTCharacterStatComponent::GetStaRatio() const
{
	TTCHECK(CurrentStatData, 0.0f);
	return CurrentStatData->MaxSta <= 0.0f ? 0.0f : CurrentSta / CurrentStatData->MaxSta;
}

float UTTCharacterStatComponent::GetAtk() const
{
	TTCHECK(CurrentStatData, 0.0f);
	return CurrentStatData->Atk;
}

float UTTCharacterStatComponent::GetDef() const
{
	TTCHECK(CurrentStatData, 0.0f);
	return CurrentStatData->Def;
}

void UTTCharacterStatComponent::SetObjectStat(FName NewObjectName)
{
	const auto& TTGameInstance{ Cast<UTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())) };
	
	if (TTGameInstance) CurrentStatData = TTGameInstance->GetTTCharacterData(NewObjectName);
	if (CurrentStatData)
	{
		ObjectName = NewObjectName;
		SetHP(CurrentStatData->MaxHP);
		SetSta(CurrentStatData->MaxSta);
		SetAtk(CurrentStatData->Atk);
		SetDef(CurrentStatData->Def);
	}
	else TTLOG(Error, TEXT("ObjectName (%s) data doesn't exist"), *NewObjectName.ToString());
}

void UTTCharacterStatComponent::SetMaxHP(float NewMaxHP)
{
	CurrentStatData->MaxHP = NewMaxHP;
}

void UTTCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, CurrentStatData->MaxHP);
	if (!CurrentHP) OnHPIsZero.Broadcast();
}

void UTTCharacterStatComponent::SetMaxSta(float NewMaxSta)
{
	CurrentStatData->MaxSta = NewMaxSta;
}

void UTTCharacterStatComponent::SetSta(float NewSta)
{
	CurrentSta = FMath::Clamp<float>(NewSta, 0.0f, CurrentStatData->MaxSta);
}

void UTTCharacterStatComponent::SetAtk(float NewAtk)
{
	CurrentStatData->Atk = NewAtk;
	if (CurrentStatData->Atk <= 0.0f) CurrentStatData->Atk = 0.0f;
}

void UTTCharacterStatComponent::SetDef(float NewDef)
{
	CurrentStatData->Def = FMath::Clamp<float>(NewDef, 0.0f, 70.0f);
}

void UTTCharacterStatComponent::SetDamage(float NewDamage)
{
	TTCHECK(CurrentStatData);
	NewDamage *= 1.0f - CurrentStatData->Def / 100.0f;
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}
