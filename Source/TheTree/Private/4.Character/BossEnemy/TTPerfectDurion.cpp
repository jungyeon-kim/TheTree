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

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_HitImpact.P_PerfectDurion_HitImpact"));
	Effect->AddEffect(TEXT("Hurricane"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_Hurricane.P_PerfectDurion_Hurricane"));
	Effect->AddEffect(TEXT("ExplosionRock"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_ExplosionRock.P_PerfectDurion_ExplosionRock"));
	Effect->AddEffect(TEXT("SummonWeapon"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_SummonWeapon.P_PerfectDurion_SummonWeapon"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_Attack_SoundQue.Durion_Attack_SoundQue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_HitAttack_SoundQue.Durion_HitAttack_SoundQue"));
	Audio->AddSoundCue(TEXT("Explosion"), TEXT("/Game/Assets/Sound/Common/Common_Explosion_SoundCue.Common_Explosion_SoundCue"));
	Audio->AddSoundWave(TEXT("ExplosionRock"), TEXT("/Game/Assets/Sound/Common/Common_ExplosionRock.Common_ExplosionRock"));
	Audio->AddSoundWave(TEXT("SummonWeapon"), TEXT("/Game/Assets/Sound/Common/Common_Casting_00.Common_Casting_00"));
	Audio->AddSoundWave(TEXT("SummonBlood"), TEXT("/Game/Assets/Sound/Common/Common_HitBlood_00.Common_HitBlood_00"));

	GetCapsuleComponent()->SetCapsuleSize(100.0f, 200.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	GeneralMoveSpeed = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 20.0f;
}

void ATTPerfectDurion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("PerfectDurion"));

	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionAttackMontage.PerfectDurionAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("ChargeAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionChargeAttackMontage.PerfectDurionChargeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("HoldAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionHoldAttackMontage.PerfectDurionHoldAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("QuakeAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionQuakeAttackMontage.PerfectDurionQuakeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("JumpAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionJumpAttackMontage.PerfectDurionJumpAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("SummonAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionSummonAttackMontage.PerfectDurionSummonAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("BackMove"), TEXT("/Game/Blueprints/Animation/BossEnemy/PerfectDurion/PerfectDurionBackMoveMontage.PerfectDurionBackMoveMontage"));
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

	FVector HitStartLocation{};
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PerfectDurionAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionHoldAttackMontage")):
		AttackLength = 500.0f;
		AttackRadius = 100.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 2000.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 450.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
		AttackLength = 200.0f;
		AttackRadius = 200.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	}

	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + HitStartLocation,
		GetActorLocation() + GetActorForwardVector() * AttackLength + HitStartLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PerfectDurionAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionHoldAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 1.5f, DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.5f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Effect->PlayEffectAtLocation(TEXT("Hurricane"), GetActorLocation(), 5.0f);
		Audio->PlaySoundCue2D(TEXT("Explosion"));
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 3.0f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 8.0f);
		Effect->PlayEffectAtLocation(TEXT("ExplosionRock"), GetActorLocation(), 5.0f);
		Audio->PlaySoundWave2D(TEXT("ExplosionRock"));
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 5.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffectAtLocation(TEXT("SummonWeapon"), GetActorLocation() + GetActorForwardVector() * AttackLength, 10.0f);
		Audio->PlaySoundWaveAtLocation(TEXT("SummonWeapon"), GetActorLocation());
		Audio->PlaySoundWaveAtLocation(TEXT("SummonBlood"), GetActorLocation());
		break;
	}
	}

	if (FTTWorld::bIsDebugging)
	{
		FVector Trace{ GetActorForwardVector() * AttackLength };
		FVector Center{ GetActorLocation() + Trace * 0.5f + HitStartLocation };
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
	case FTTWorld::HashCode(TEXT("PerfectDurionHoldAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionBackMoveMontage")):
		OnDodgeEnded.Broadcast();
		break;
	}
}