#include "TTImperfectDurion.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTAIStatComponent.h"
#include "DrawDebugHelpers.h"

ATTImperfectDurion::ATTImperfectDurion()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BossEnemy/ImperfectDurion/SK_Imperfect_Durion.SK_Imperfect_Durion") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionAnimBlueprint.ImperfectDurionAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("AttackImpact"), TEXT("/Game/Assets/Effect/Particle/P_ImperfectDurion_AttackImpact.P_ImperfectDurion_AttackImpact"));
	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_HitImpact.P_PerfectDurion_HitImpact"));
	Effect->AddEffect(TEXT("Drain"), TEXT("/Game/Assets/Effect/Particle/P_ImperfectDurion_Drain.P_ImperfectDurion_Drain"));
	Effect->AddEffect(TEXT("BigHand"), TEXT("/Game/Assets/Effect/Particle/P_ImperfectDurion_BigHand.P_ImperfectDurion_BigHand"));
	Effect->AddEffect(TEXT("Recovery"), TEXT("/Game/Assets/Effect/Particle/P_ImperfectDurion_Recovery.P_ImperfectDurion_Recovery"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_HitAttack_SoundQue.Durion_HitAttack_SoundQue"));
	Audio->AddSoundCue(TEXT("Explosion"), TEXT("/Game/Assets/Sound/Common/Common_Explosion_SoundCue.Common_Explosion_SoundCue"));
	Audio->AddSoundWave(TEXT("Attack"), TEXT("/Game/Assets/Sound/BossEnemy/ImperfectDurion/ImperfectDurion_Attack.ImperfectDurion_Attack"));
	Audio->AddSoundWave(TEXT("Drain"), TEXT("/Game/Assets/Sound/BossEnemy/ImperfectDurion/ImperfectDurion_Drain.ImperfectDurion_Drain"));
	Audio->AddSoundWave(TEXT("BigHand"), TEXT("/Game/Assets/Sound/BossEnemy/ImperfectDurion/ImperfectDurion_BigHand.ImperfectDurion_BigHand"));

	GetCapsuleComponent()->SetCapsuleSize(100.0f, 200.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	GeneralMoveSpeed = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 5.0f;
}

void ATTImperfectDurion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIStat->SetObjectStat(TEXT("ImperfectDurion"), GetGameInstance());
	
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionAttackMontage.ImperfectDurionAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("DrainAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionDrainAttackMontage.ImperfectDurionDrainAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("ChargeAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionChargeAttackMontage.ImperfectDurionChargeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("QuakeAttack"), TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionQuakeAttackMontage.ImperfectDurionQuakeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("Teleport"), TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionTeleportMontage.ImperfectDurionTeleportMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTImperfectDurion::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTImperfectDurion::AttackCheck);

	AIStat->OnHPIsZero.AddLambda([&]() { SetPlayRate(0.0f, 0.15f, 0.1f); });
}

void ATTImperfectDurion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ImperfectDurion.BT_ImperfectDurion"));
}

void ATTImperfectDurion::BeginPlay()
{
	Super::BeginPlay();
}

void ATTImperfectDurion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTImperfectDurion::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - AIStat->GetDef() / 100.0f));

	return FinalDamage;
}

void ATTImperfectDurion::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
		AttackLength = 500.0f;
		AttackRadius = 100.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("ImperfectDurionDrainAttackMontage")):
		AttackLength = 450.0f;
		AttackRadius = 150.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("ImperfectDurionChargeAttackMontage")):
		AttackLength = 1000.0f;
		AttackRadius = 200.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("ImperfectDurionQuakeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 1600.0f;
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
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(AIStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffectAtLocation(TEXT("AttackImpact"), GetActorLocation() + GetActorForwardVector() * AttackLength / 1.15f, 5.0f);
		Audio->PlaySoundWaveAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("ImperfectDurionDrainAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(AIStat->GetAtk(), DamageEvent, GetController(), this);
				AIStat->SetHP(AIStat->GetHP() + AIStat->GetAtk() * 2.0f);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Effect->PlayEffectAtLocation(TEXT("Recovery"), GetActorLocation(), FVector(3.0f, 3.0f, 5.0f));
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffectAtLocation(TEXT("Drain"), GetActorLocation() + GetActorForwardVector() * AttackLength / 1.25f, 5.0f);
		Audio->PlaySoundWaveAtLocation(TEXT("Drain"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("ImperfectDurionChargeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(AIStat->GetAtk() * 3.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 10.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffectAtLocation(TEXT("BigHand"), GetActorLocation() + GetActorForwardVector() * 400.0f, 
			GetActorForwardVector().Rotation(), FVector(3.5f, 5.0f, 3.5f));
		Audio->PlaySoundWaveAtLocation(TEXT("BigHand"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("ImperfectDurionQuakeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(AIStat->GetAtk() * 4.0f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Audio->PlaySoundCue2D(TEXT("Explosion"));
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

void ATTImperfectDurion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
	case FTTWorld::HashCode(TEXT("ImperfectDurionDrainAttackMontage")):
	case FTTWorld::HashCode(TEXT("ImperfectDurionChargeAttackMontage")):
	case FTTWorld::HashCode(TEXT("ImperfectDurionQuakeAttackMontage")):
		OnAttackEnded.Broadcast();
	case FTTWorld::HashCode(TEXT("ImperfectDurionTeleportMontage")):
		OnTeleportEnded.Broadcast();
		break;
	}
}