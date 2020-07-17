#include "TTAIStatComponent.h"
#include "TTGameInstance.h"

UTTAIStatComponent::UTTAIStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UTTAIStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UTTAIStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTTAIStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FName UTTAIStatComponent::GetObjectName() const
{
	TTCHECK(TTAIData, NAME_None);
	return ObjectName;
}

float UTTAIStatComponent::GetHPRatio() const
{
	TTCHECK(TTAIData, 0.0f);
	return CurrentMaxHP <= 0.0f ? 0.0f : CurrentHP / CurrentMaxHP;
}

float UTTAIStatComponent::GetMaxHP() const
{
	TTCHECK(TTAIData, 0.0f);
	return CurrentMaxHP;
}

float UTTAIStatComponent::GetHP() const
{
	TTCHECK(TTAIData, 0.0f);
	return CurrentHP;
}

float UTTAIStatComponent::GetAtk() const
{
	TTCHECK(TTAIData, 0.0f);
	return CurrentAtk;
}

float UTTAIStatComponent::GetDef() const
{
	TTCHECK(TTAIData, 0.0f);
	return CurrentDef;
}

void UTTAIStatComponent::SetObjectStat(FName NewObjectName, UGameInstance* GameInst)
{
	TTCHECK(GameInst);
	const auto TTGameInstance{ Cast<UTTGameInstance>(GameInst) };
	if (TTGameInstance) TTAIData = TTGameInstance->GetTTAIData(NewObjectName);

	if (TTAIData)
	{
		ObjectName = NewObjectName;
		SetMaxHP(TTAIData->MaxHP);
		SetHP(TTAIData->MaxHP);
		SetAtk(TTAIData->Atk);
		SetDef(TTAIData->Def);
	}
	else TTLOG(Error, TEXT("ObjectName (%s) data doesn't exist"), *NewObjectName.ToString());
}

void UTTAIStatComponent::SetMaxHP(float NewMaxHP)
{
	CurrentMaxHP = NewMaxHP;
}

void UTTAIStatComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, CurrentMaxHP);
	if (!CurrentHP) OnHPIsZero.Broadcast();
}

void UTTAIStatComponent::SetAtk(float NewAtk)
{
	CurrentAtk = NewAtk;
	if (CurrentAtk <= 0.0f) CurrentAtk = 0.0f;
}

void UTTAIStatComponent::SetDef(float NewDef)
{
	CurrentDef = FMath::Clamp<float>(NewDef, 0.0f, 70.0f);
}

void UTTAIStatComponent::SetDamage(float NewDamage)
{
	TTCHECK(TTAIData);
	NewDamage *= 1.0f - CurrentDef / 100.0f;
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentMaxHP));
}