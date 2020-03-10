#include "TTEnemyAnimInstance.h"

UTTEnemyAnimInstance::UTTEnemyAnimInstance()
{
}

void UTTEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

void UTTEnemyAnimInstance::PlayMontage(EMontageType MontageType)
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
	case EMontageType::ATTACK_QUAKE:
		TTCHECK(!bIsDead && QuakeAttackMontage);
		Montage_Play(QuakeAttackMontage, 1.0f);
		break;
	case EMontageType::ATTACK_JUMP:
		TTCHECK(!bIsDead && JumpAttackMontage);
		Montage_Play(JumpAttackMontage, 1.0f);
		break;
	case EMontageType::ATTACK_SUMMON:
		TTCHECK(!bIsDead && SummonAttackMontage);
		Montage_Play(SummonAttackMontage, 1.0f);
		break;
	case EMontageType::DEFENSE:
		TTCHECK(!bIsDead && DefenseMontage);
		Montage_Play(DefenseMontage, 1.0f);
		break;
	}
}

void UTTEnemyAnimInstance::SetDamaged()
{
	bIsDamaged = true;
}

void UTTEnemyAnimInstance::SetDead()
{
	bIsDead = true;
}

void UTTEnemyAnimInstance::SetMontage(EMontageType MontageType, const TCHAR* MontagePath)
{
	switch (MontageType)
	{
	case EMontageType::ATTACK:
		AttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::ATTACK_CHARGE:
		ChargeAttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::ATTACK_QUAKE:
		QuakeAttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::ATTACK_JUMP:
		JumpAttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::ATTACK_SUMMON:
		SummonAttackMontage = LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	case EMontageType::DEFENSE:
		DefenseMontage= LoadObject<UAnimMontage>(NULL, MontagePath);
		break;
	}
}

void UTTEnemyAnimInstance::AnimNotify_AttackStart()
{
	OnAttackStart.Broadcast();
}

void UTTEnemyAnimInstance::AnimNotify_DefenseStart()
{
	OnDefenseStart.Broadcast();
}

void UTTEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UTTEnemyAnimInstance::AnimNotify_PlayTalk()
{
	OnPlayTalk.Broadcast();
}
