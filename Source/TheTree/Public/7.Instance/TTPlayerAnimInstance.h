#pragma once

#include "TheTree.h"
#include "Animation/AnimInstance.h"
#include "TTPlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSwapWeaponDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayAttackVoiceDelegate);

UCLASS()
class THETREE_API UTTPlayerAnimInstance : public UAnimInstance
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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InOutWeapon", Meta = (AllowPrivateAccess = true))
	UAnimMontage* InWeaponMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "InOutWeapon", Meta = (AllowPrivateAccess = true))
	UAnimMontage* OutWeaponMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "KnockBack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* KnockBackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Death", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	UFUNCTION()
	void AnimNotify_SwapWeapon();
	UFUNCTION()
	void AnimNotify_PlayAttackVoice();
	FName GetAttackMontageSectionName(int32 Section) const;
public:
	FOnAttackHitCheckDelegate OnAttackHitCheck{};
	FOnNextAttackCheckDelegate OnNextAttackCheck{};
	FOnSwapWeaponDelegate OnSwapWeapon{};
	FOnPlayAttackVoiceDelegate OnPlayAttackVoice{};
public:
	UTTPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(EMontageType MontageType);
	void JumpToAttackMontageSection(int32 NewSection);

	bool GetIsBattleOn() const;
	void SetIsBattleOn(bool Flag);
	void SetDead();
};
