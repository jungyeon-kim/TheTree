#include "TTBasicEnemyAnimInstance.h"

UTTBasicEnemyAnimInstance::UTTBasicEnemyAnimInstance()
{
}

void UTTBasicEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto& Pawn{ TryGetPawnOwner() };
	if (IsValid(Pawn) && !bIsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		const auto& Character{ Cast<ACharacter>(Pawn) };
		if (Character) bIsOnAir = Character->GetMovementComponent()->IsFalling();
	}

	if (GetCurrentStateName(GetStateMachineIndex(FName("BaseAction"))) == FName("HitReact")) bIsDamaged = false;
}

void UTTBasicEnemyAnimInstance::PlayMontage(EMontageType MontageType)
{
	switch (MontageType)
	{
	case EMontageType::ATTACK:
		TTCHECK(!bIsDead && AttackMontage);
		Montage_Play(AttackMontage, 1.0f);
		break;
	case EMontageType::ATTACK_CHARGE:
		TTCHECK(!bIsDead && ChargeAttackMontage);
		Montage_Play(ChargeAttackMontage, 1.0f);
		break;
	}
}

void UTTBasicEnemyAnimInstance::SetDamaged()
{
	bIsDamaged = true;
}

void UTTBasicEnemyAnimInstance::SetDead()
{
	bIsDead = true;
}

void UTTBasicEnemyAnimInstance::SetMontage(EMontageType MontageType, const TCHAR* MontagePath)
{
	switch (MontageType)
	{
	case EMontageType::ATTACK:
		AttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::ATTACK_CHARGE:
		ChargeAttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	}
}

void UTTBasicEnemyAnimInstance::AnimNotify_AttackStart()
{
	OnAttackStart.Broadcast();
}

void UTTBasicEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}