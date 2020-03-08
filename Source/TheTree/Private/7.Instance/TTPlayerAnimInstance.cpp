#include "TTPlayerAnimInstance.h"

UTTPlayerAnimInstance::UTTPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerAttackMontage.PlayerAttackMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerDodgeMontage.PlayerDodgeMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> INWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerInWeaponMontage.PlayerInWeaponMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OUTWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerOutWeaponMontage.PlayerOutWeaponMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KNOCKBACK_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerKnockBackMontage.PlayerKnockBackMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerDeathMontage.PlayerDeathMontage") };
	if (ATTACK_MONTAGE.Succeeded()) AttackMontage = ATTACK_MONTAGE.Object;
	if (DODGE_MONTAGE.Succeeded()) DodgeMontage = DODGE_MONTAGE.Object;
	if (INWEAPON_MONTAGE.Succeeded()) InWeaponMontage = INWEAPON_MONTAGE.Object;
	if (OUTWEAPON_MONTAGE.Succeeded()) OutWeaponMontage = OUTWEAPON_MONTAGE.Object;
	if (KNOCKBACK_MONTAGE.Succeeded()) KnockBackMontage = KNOCKBACK_MONTAGE.Object;
	if (DEATH_MONTAGE.Succeeded()) DeathMontage = DEATH_MONTAGE.Object;
}

void UTTPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

void UTTPlayerAnimInstance::PlayMontage(EMontageType MontageType)
{
	switch (MontageType)
	{
	case EMontageType::ATTACK:
		TTCHECK(!bIsDead && AttackMontage);
		Montage_Play(AttackMontage, 1.0f);
		break;
	case EMontageType::DODGE:
		TTCHECK(!bIsDead && DodgeMontage);
		Montage_Play(DodgeMontage, 1.0f);
		break;
	case EMontageType::INOUTWEAPON:
		TTCHECK(!bIsDead && InWeaponMontage && OutWeaponMontage);
		if (bIsBattleOn)
		{
			Montage_Play(InWeaponMontage, 1.0f);
			bIsBattleOn = false;
		}
		else
		{
			Montage_Play(OutWeaponMontage, 1.0f);
			bIsBattleOn = true;
		}
		break;
	case EMontageType::KNOCKBACK:
		TTCHECK(!bIsDead && KnockBackMontage);
		Montage_Play(KnockBackMontage, 1.0f);
		break;
	case EMontageType::DEATH:
		TTCHECK(!bIsDead && DeathMontage);
		Montage_Play(DeathMontage, 1.0f);
		break;
	}
}

void UTTPlayerAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	TTCHECK(!bIsDead && AttackMontage);
	TTCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTTPlayerAnimInstance::SetDead()
{
	bIsDead = true;
}

void UTTPlayerAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UTTPlayerAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTTPlayerAnimInstance::AnimNotify_SwapWeapon()
{
	OnSwapWeapon.Broadcast();
}

void UTTPlayerAnimInstance::AnimNotify_PlaySound()
{
	OnPlaySound.Broadcast();
}

FName UTTPlayerAnimInstance::GetAttackMontageSectionName(int32 Section) const
{
	TTCHECK(FMath::IsWithinInclusive(Section, 1, 4), NAME_None);
	return *FString::Printf(TEXT("Attack%d"), Section);
}

bool UTTPlayerAnimInstance::GetIsBattleOn() const
{
	return bIsBattleOn;
}