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
	struct FTTCharacterData* TTCharacterData{};

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FName ObjectName;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Gold;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float MaxSta;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentSta;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float StaToGetPerHit;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float Atk;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float Def;
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	FOnHPIsZeroDelegate OnHPIsZero{};
public:
	UTTCharacterStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FName GetObjectName() const;
	int32 GetGold() const;
	float GetHPRatio() const;
	float GetStaRatio() const;
	float GetMaxHP() const;
	float GetHP() const;
	float GetMaxSta() const;
	float GetSta() const;
	float GetStaToGetPerHit() const;
	float GetAtk() const;
	float GetDef() const;
	void SetObjectStat(FName NewObjectName, class UGameInstance* GameInst);
	void SetGold(int32 NewGold);
	void SetMaxHP(float NewMaxHP);
	void SetHP(float NewHP);
	void SetMaxSta(float NewMaxSta);
	void SetSta(float NewSta);
	void SetStaToGetPerHit(float NewStaToGetPerHit);
	void SetAtk(float NewAtk);
	void SetDef(float NewDef);
	void SetDamage(float NewDamage);
};
