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
	Audio->AddSoundCue(TEXT("Talk"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/PerfectDurion_Talk_SoundQue.PerfectDurion_Talk_SoundQue"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_Attack_SoundQue.Durion_Attack_SoundQue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_HitAttack_SoundQue.Durion_HitAttack_SoundQue"));
	Audio->AddSoundCue(TEXT("Explosion"), TEXT("/Game/Assets/Sound/Common/Common_Explosion_SoundCue.Common_Explosion_SoundCue"));
	Audio->AddSound(TEXT("ExplosionRock"), TEXT("/Game/Assets/Sound/Common/Common_ExplosionRock.Common_ExplosionRock"));
	Audio->AddSound(TEXT("SummonWeapon"), TEXT("/Game/Assets/Sound/Common/Common_Casting_00.Common_Casting_00"));
	Audio->AddSound(TEXT("SummonBlood"), TEXT("/Game/Assets/Sound/Common/Common_HitBlood_00.Common_HitBlood_00"));

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
	TTAnimInstance->OnPlayTalk.AddLambda([&]()
	{
		Audio->PlaySoundCueAtLocation(TEXT("Talk"), GetActorLocation());
	});
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
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
		AttackLength = 600.0f;
		AttackRadius = 100.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionQuakeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 2000.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 400.0f;
		break;
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
		AttackLength = 400.0f;
		AttackRadius = 200.0f;
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
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionChargeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 1.5f, DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
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
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.0f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Effect->PlayEffect(TEXT("Hurricane"), GetActorLocation(), 5.0f);
		Audio->PlaySoundCue2D(TEXT("Explosion"));
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionJumpAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.5f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 10.0f);
		Effect->PlayEffect(TEXT("ExplosionRock"), GetActorLocation(), 5.0f);
		Audio->PlaySound2D(TEXT("ExplosionRock"));
		break;
	}
	case FTTWorld::HashCode(TEXT("PerfectDurionSummonAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 3.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffect(TEXT("SummonWeapon"), GetActorLocation() + GetActorForwardVector() * AttackLength, 10.0f);
		Audio->PlaySoundAtLocation(TEXT("SummonWeapon"), GetActorLocation());
		Audio->PlaySoundAtLocation(TEXT("SummonBlood"), GetActorLocation());
		break;
	}
	}

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