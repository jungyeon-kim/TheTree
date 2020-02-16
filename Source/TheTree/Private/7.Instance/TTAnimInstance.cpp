#include "TTAnimInstance.h"

UTTAnimInstance::UTTAnimInstance()
{
}

void UTTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

void UTTAnimInstance::PlayAttackMontange()
{
}

FName UTTAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName();
}

void UTTAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
}

void UTTAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UTTAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTTAnimInstance::SetDeadAnim()
{
	bIsDead = true;
}
