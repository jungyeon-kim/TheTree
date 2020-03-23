#include "TTPlayerAnimInstance.h"

UTTPlayerAnimInstance::UTTPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerAttackMontage.PlayerAttackMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DODGE_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerDodgeMontage.PlayerDodgeMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> INWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerInWeaponMontage.PlayerInWeaponMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OUTWEAPON_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerOutWeaponMontage.PlayerOutWeaponMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KNOCKBACK_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerKnockBackMontage.PlayerKnockBackMontage") };
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE{ TEXT("/Game/Blueprints/Animation/Player/PlayerDeathMontage.PlayerDeathMontage") };
	if (ATTACK_MONTAGE.Succeeded()) Montage.Emplace(TEXT("BasicAttack"), ATTACK_MONTAGE.Object);
	if (DODGE_MONTAGE.Succeeded()) Montage.Emplace(TEXT("Dodge"), DODGE_MONTAGE.Object);
	if (INWEAPON_MONTAGE.Succeeded()) Montage.Emplace(TEXT("InWeapon"), INWEAPON_MONTAGE.Object);
	if (OUTWEAPON_MONTAGE.Succeeded()) Montage.Emplace(TEXT("OutWeapon"), OUTWEAPON_MONTAGE.Object);
	if (KNOCKBACK_MONTAGE.Succeeded()) Montage.Emplace(TEXT("KnockBack"), KNOCKBACK_MONTAGE.Object);
	if (DEATH_MONTAGE.Succeeded()) Montage.Emplace(TEXT("Death"), DEATH_MONTAGE.Object);
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

void UTTPlayerAnimInstance::PlayMontage(FName MontageType)
{
	TTCHECK(!bIsDead);
	if (Montage.Find(MontageType)) Montage_Play(Montage[MontageType]);
	else TTLOG(Error, TEXT("Can't find MontageType"));
}

void UTTPlayerAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	TTCHECK(!bIsDead && Montage[TEXT("BasicAttack")]);
	TTCHECK(Montage_IsPlaying(Montage[TEXT("BasicAttack")]));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), Montage[TEXT("BasicAttack")]);
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

FName UTTPlayerAnimInstance::GetAttackMontageSectionName(int32 Section) const
{
	TTCHECK(FMath::IsWithinInclusive(Section, 1, 4), NAME_None);
	return *FString::Printf(TEXT("Attack%d"), Section);
}

bool UTTPlayerAnimInstance::GetIsBattleOn() const
{
	return bIsBattleOn;
}

void UTTPlayerAnimInstance::SetIsBattleOn(bool Flag)
{
	bIsBattleOn = Flag;
}
