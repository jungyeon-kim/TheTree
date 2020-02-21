#pragma once

#include "TheTree.h"
#include "Animation/AnimInstance.h"
#include "TTAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSwapWeaponDelegate);

UCLASS()
class THETREE_API UTTAnimInstance : public UAnimInstance
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
private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	UFUNCTION()
	void AnimNotify_SwapWeapon();
	FName GetAttackMontageSectionName(int32 Section) const;
public:
	FOnNextHitCheckDelegate OnAttackHitCheck{};
	FOnNextAttackCheckDelegate OnNextAttackCheck{};
	FOnSwapWeaponDelegate OnSwapWeapon{};
public:
	UTTAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontange();
	void JumpToAttackMontageSection(int32 NewSection);
	void PlayDodgeMontage();
	void PlayInOutWeaponMontage();

	bool GetIsBattleOn() const;
	void SetDeadAnim();
};
