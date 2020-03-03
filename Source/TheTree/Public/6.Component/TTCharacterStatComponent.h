#pragma once

#include "TheTree.h"
#include "Components/ActorComponent.h"
#include "TTCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THETREE_API UTTCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FTTCharacterData* CurrentStatData{};

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FName ObjectName;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentSta;
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	FOnHPIsZeroDelegate OnHPIsZero{};
public:
	UTTCharacterStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FName GetObjectName() const;
	float GetHPRatio() const;
	float GetStaRatio() const;
	float GetAtk() const;
	float GetDef() const;
	void SetObjectStat(FName NewObjectName);
	void SetMaxHP(float NewMaxHP);
	void SetHP(float NewHP);
	void SetMaxSta(float NewMaxSta);
	void SetSta(float NewSta);
	void SetAtk(float NewAtk);
	void SetDef(float NewDef);
	void SetDamage(float NewDamage);
};
