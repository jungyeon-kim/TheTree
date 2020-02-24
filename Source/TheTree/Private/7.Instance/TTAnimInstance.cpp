#include "TTAnimInstance.h"

UTTAnimInstance::UTTAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerAttackMontage.PlayerAttackMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerDodgeMontage.PlayerDodgeMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> INWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerInWeaponMontage.PlayerInWeaponMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OUTWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayeroutWeaponMontage.PlayerOutWeaponMontage") };
	if (ATTACK_MONTAGE.Succeeded()) AttackMontage = ATTACK_MONTAGE.Object;
	if (DODGE_MONTAGE.Succeeded()) DodgeMontage = DODGE_MONTAGE.Object;
	if (INWEAPON_MONTAGE.Succeeded()) InWeaponMontage = INWEAPON_MONTAGE.Object;
	if (OUTWEAPON_MONTAGE.Succeeded()) OutWeaponMontage = OUTWEAPON_MONTAGE.Object;
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
	TTCHECK(!bIsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UTTAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	TTCHECK(!bIsDead);
	TTCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTTAnimInstance::PlayDodgeMontage()
{
	TTCHECK(!bIsDead);
	Montage_Play(DodgeMontage, 1.0f);
}

void UTTAnimInstance::PlayInOutWeaponMontage()
{
	TTCHECK(!bIsDead);
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
}

bool UTTAnimInstance::GetIsBattleOn() const
{
	return bIsBattleOn;
}

void UTTAnimInstance::SetDeadAnim()
{
	bIsDead = true;
}

void UTTAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UTTAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTTAnimInstance::AnimNotify_SwapWeapon()
{
	OnSwapWeapon.Broadcast();
}

FName UTTAnimInstance::GetAttackMontageSectionName(int32 Section) const
{
	TTCHECK(FMath::IsWithinInclusive(Section, 1, 4), NAME_None);
	return *FString::Printf(TEXT("Attack%d"), Section);
}