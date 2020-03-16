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
	TTCHECK(!bIsDead);
	if (Montage.Find(MontageType)) Montage_Play(Montage[MontageType]);
	else TTLOG(Error, TEXT("Can't find MontageType"));
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
	const auto& MontageCheck{ LoadObject<UAnimMontage>(this, MontagePath) };

	if (MontageCheck->IsValidLowLevel()) Montage.Emplace(MontageType, MontageCheck);
	else TTLOG(Error, TEXT("Can't find MontagePath (%s)"), MontagePath);
}

void UTTEnemyAnimInstance::AnimNotify_AttackStart()
{
	OnAttackStart.Broadcast();
}

void UTTEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}