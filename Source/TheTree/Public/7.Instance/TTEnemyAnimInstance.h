#pragma once

#include "TheTree.h"
#include "Animation/AnimInstance.h"
#include "TTEnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDefenseStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayTalkDelegate);

UCLASS()
class THETREE_API UTTEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsOnAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsDamaged;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bIsBattleOn;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ChargeAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* QuakeAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SummonAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Defense", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DefenseMontage;
private:
	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_DefenseStart();
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_PlayTalk();
public:
	FOnAttackStartDelegate OnAttackStart{};
	FOnDefenseStartDelegate OnDefenseStart{};
	FOnAttackHitCheckDelegate OnAttackHitCheck{};
	FOnPlayTalkDelegate OnPlayTalk{};
public:
	UTTEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(EMontageType MontageType);

	void SetDamaged();
	void SetDead();
	void SetMontage(EMontageType MontageType, const TCHAR* MontagePath);
};