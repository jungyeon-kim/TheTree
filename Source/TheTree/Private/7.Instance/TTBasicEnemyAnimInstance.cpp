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
}

void UTTBasicEnemyAnimInstance::PlayAttackMontange()
{
	TTCHECK(!bIsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UTTBasicEnemyAnimInstance::SetDeadAnim()
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
	}
}

void UTTBasicEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}