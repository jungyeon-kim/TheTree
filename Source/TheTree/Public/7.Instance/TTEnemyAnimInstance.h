#pragma once

#include "TheTree.h"
#include "Animation/AnimInstance.h"
#include "TTEnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartInitDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEndInitDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class THETREE_API UTTEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool bIsDead;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Montage", Meta = (AllowPrivateAccess = true))
	TMap<FName, UAnimMontage*> Montage;
private:
	UFUNCTION()
	void AnimNotify_StartInit();
	UFUNCTION()
	void AnimNotify_EndInit();
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
	UFUNCTION()
	void AnimNotify_SetCollisionToEnemy();
	UFUNCTION()
	void AnimNotify_SetCollisionToIgnoreOnlyPawn();
public:
	FOnStartInitDelegate OnStartInit{};
	FOnEndInitDelegate OnEndInit{};
	FOnAttackHitCheckDelegate OnAttackHitCheck{};
public:
	UTTEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayMontage(FName MontageType);

	void SetDead();
	void SetMontage(FName MontageType, const TCHAR* MontagePath);
};