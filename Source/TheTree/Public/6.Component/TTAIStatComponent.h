#pragma once

#include "TheTree.h"
#include "Components/ActorComponent.h"
#include "TTAIStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THETREE_API UTTAIStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FTTAIData* TTAIData{};

	UPROPERTY(VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FName ObjectName;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMaxHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentAtk;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentDef;
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	FOnHPIsZeroDelegate OnHPIsZero{};
public:
	UTTAIStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FName GetObjectName() const;
	float GetHPRatio() const;
	float GetMaxHP() const;
	float GetHP() const;
	float GetAtk() const;
	float GetDef() const;
	void SetObjectStat(FName NewObjectName, class UGameInstance* GameInst);
	void SetMaxHP(float NewMaxHP);
	void SetHP(float NewHP);
	void SetAtk(float NewAtk);
	void SetDef(float NewDef);
	void SetDamage(float NewDamage);
};
