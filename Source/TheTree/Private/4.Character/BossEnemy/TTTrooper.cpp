#include "TTTrooper.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTTrooper::ATTTrooper()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BossEnemy/Trooper/SK_ArchakalashTrooper.SK_ArchakalashTrooper") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperAnimBlueprint.TrooperAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_HitImpact.P_PerfectDurion_HitImpact"));
	Effect->AddEffect(TEXT("ExplosionRock"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_ExplosionRock.P_PerfectDurion_ExplosionRock"));
	Effect->AddEffect(TEXT("Laser"), TEXT("/Game/Assets/Effect/Particle/P_Trooper_Shot.P_Trooper_Shot"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_Attack_SoundQue.Durion_Attack_SoundQue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_HitAttack_SoundQue.Durion_HitAttack_SoundQue"));
	Audio->AddSoundCue(TEXT("Explosion"), TEXT("/Game/Assets/Sound/Common/Common_Explosion_SoundCue.Common_Explosion_SoundCue"));
	Audio->AddSoundWave(TEXT("Laser"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaArcher/ArcdevaArcher_AttackStart.ArcdevaArcher_AttackStart"));

	GetCapsuleComponent()->SetCapsuleSize(200.0f, 200.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	GeneralMoveSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 20.0f;
}

void ATTTrooper::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("Trooper"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("BasicAttack1"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperAttackMontage_00.TrooperAttackMontage_00"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack2"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperAttackMontage_01.TrooperAttackMontage_01"));
	TTAnimInstance->SetMontage(TEXT("QuakeAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperQuakeAttackMontage.TrooperQuakeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("RoundAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperRoundAttackMontage.TrooperRoundAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("MoveAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperMoveAttackMontage.TrooperMoveAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("RangeAttack1"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperRangeAttackMontage_00.TrooperRangeAttackMontage_00"));
	TTAnimInstance->SetMontage(TEXT("RangeAttack2"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperRangeAttackMontage_01.TrooperRangeAttackMontage_01"));
	TTAnimInstance->SetMontage(TEXT("LaserAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/Trooper/TrooperLaserAttackMontage.TrooperLaserAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTTrooper::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTTrooper::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([&]() { SetPlayRate(0.0f, 0.15f, 0.1f); });
}

void ATTTrooper::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_Trooper.BT_Trooper"));
}

void ATTTrooper::BeginPlay()
{
	Super::BeginPlay();
}

void ATTTrooper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTTrooper::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	return FinalDamage;
}

void ATTTrooper::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_00")):
		AttackLength = 500.0f;
		AttackRadius = 600.0f;
		HitStartLocation = GetActorRightVector() * AttackRadius / 1.5f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_01")):
		AttackLength = 500.0f;
		AttackRadius = 600.0f;
		HitStartLocation = -GetActorRightVector() * AttackRadius / 1.5f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperQuakeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 600.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("TrooperRoundAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 1200.0f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperMoveAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 600.0f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_00")):
		AttackLength = 1.0f;
		AttackRadius = 600.0f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_01")):
		AttackLength = 1.0f;
		AttackRadius = 1100.0f;
		break;
	case FTTWorld::HashCode(TEXT("TrooperLaserAttackMontage")):
		AttackLength = 4900.0f;
		AttackRadius = 100.0f;
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
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_00")):
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_01")):
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
	case FTTWorld::HashCode(TEXT("TrooperQuakeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.0f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Effect->PlayEffectAtLocation(TEXT("ExplosionRock"), GetActorLocation() + HitStartLocation, 3.0f);
		Audio->PlaySoundCueAtLocation(TEXT("Explosion"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("TrooperRoundAttackMontage")):
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
	case FTTWorld::HashCode(TEXT("TrooperMoveAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 4.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 10.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		break;
	}
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_00")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		break;
	}
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_01")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 3.0f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Audio->PlaySoundCue2D(TEXT("Explosion"));
		break;
	}
	case FTTWorld::HashCode(TEXT("TrooperLaserAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 5.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffectAtLocation(TEXT("Laser"), GetActorLocation(), GetActorForwardVector().Rotation(), 
			FVector(10.0f, 3.0f, 3.0f));
		Audio->PlaySoundWave2D(TEXT("Laser"));
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

void ATTTrooper::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_00")):
	case FTTWorld::HashCode(TEXT("TrooperAttackMontage_01")):
	case FTTWorld::HashCode(TEXT("TrooperQuakeAttackMontage")):
	case FTTWorld::HashCode(TEXT("TrooperRoundAttackMontage")):
	case FTTWorld::HashCode(TEXT("TrooperMoveAttackMontage")):
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_00")):
	case FTTWorld::HashCode(TEXT("TrooperRangeAttackMontage_01")):
	case FTTWorld::HashCode(TEXT("TrooperLaserAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}