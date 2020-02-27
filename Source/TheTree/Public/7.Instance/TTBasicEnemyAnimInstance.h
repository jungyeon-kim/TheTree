#pragma once

#include "TheTree.h"
#include "Animation/AnimInstance.h"
#include "TTBasicEnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

enum class EMontageType
{
	ATTACK, HITREACT
};

UCLASS()
class THETREE_API UTTBasicEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsOnAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bIsBattleOn;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Hit", Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitReactMontage;
private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
public:
	FOnAttackHitCheckDelegate OnAttackHitCheck{};
public:
	UTTBasicEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontange();
	void PlayHitReactMontange();

	void SetDeadAnim();
	void SetMontage(EMontageType MontageType, const TCHAR* MontagePath);
};