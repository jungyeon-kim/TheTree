#include "TTPerfectDurion.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTPerfectDurion::ATTPerfectDurion()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BossEnemy/PerfectDurion/SK_Perfect_Durion.SK_Perfect_Durion") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionAnimBlueprint.PerfectDurionAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaWarrior_HitImpact.P_ArcdevaWarrior_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaWarrior/ArcdevaWarrior_Attack_SoundCue.ArcdevaWarrior_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaWarrior/ArcdevaWarrior_HitAttack_SoundCue.ArcdevaWarrior_HitAttack_SoundCue"));

	GetCapsuleComponent()->SetCapsuleSize(100.0f, 200.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	GeneralMoveSpeed = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
}

void ATTPerfectDurion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(FName("PerfectDurion"));

	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionAttackMontage.PerfectDurionAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::ATTACK_CHARGE, TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionChargeAttackMontage.PerfectDurionChargeAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::ATTACK_QUAKE, TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionQuakeAttackMontage.PerfectDurionQuakeAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::ATTACK_JUMP, TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionJumpAttackMontage.PerfectDurionJumpAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::ATTACK_SUMMON, TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionSummonAttackMontage.PerfectDurionSummonAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPerfectDurion::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTPerfectDurion::AttackCheck);
}

void ATTPerfectDurion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_PerfectDurion.BT_PerfectDurion"));
}

void ATTPerfectDurion::BeginPlay()
{
	Super::BeginPlay();
}

void ATTPerfectDurion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTPerfectDurion::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	return FinalDamage;
}

void ATTPerfectDurion::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PerfectDurionAttackMontage")):
		AttackLength = 600.0f;
		AttackRadius = 100.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
		break;
	}

	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackLength,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
			{
			case FTTWorld::HashCode(TEXT("PerfectDurionAttackMontage")):
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 2.5f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
				break;
			}
			case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
			{
				break;
			}
			case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
			{
				break;
			}
			case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
			{
				break;
			}
			case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
			{
				break;
			}
			}
		}
	Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());

	if (FTTWorld::bIsDebugging)
	{
		FVector Trace{ GetActorForwardVector() * AttackLength };
		FVector Center{ GetActorLocation() + Trace * 0.5f };
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
		FColor DrawColor{ bResult ? FColor::Blue : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTPerfectDurion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PerfectDurionAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}