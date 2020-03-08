#include "TTArcdevaLancer.h"
#include "TTBasicEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArcdevaLancer::ATTArcdevaLancer()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArcdevaLancer/SK_Arcdeva_Lancer.SK_Arcdeva_Lancer") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaLancer/ArcdevaLancerAnimBlueprint.ArcdevaLancerAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaWarrior_HitImpact.P_ArcdevaWarrior_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_Attack_SoundCue.ArcdevaLancer_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitAttack_SoundCue.ArcdevaLancer_HitAttack_SoundCue"));
	Audio->AddSound(TEXT("HitChargeAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitChargeAttack.ArcdevaLancer_HitChargeAttack"));
	Audio->AddSound(TEXT("ShieldDefenseStart"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_ShieldDefenceStart.ArcdevaLancer_ShieldDefenceStart"));
	Audio->AddSound(TEXT("ShieldDefense"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_ShieldDefence.ArcdevaLancer_ShieldDefence"));

	GeneralMoveSpeed = 350.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
}

void ATTArcdevaLancer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(FName("ArcdevaLancer"));

	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaLancer/ArcdevaLancerAttackMontage.ArcdevaLancerAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::ATTACK_CHARGE, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaLancer/ArcdevaLancerChargeAttackMontage.ArcdevaLancerChargeAttackMontage"));
	TTAnimInstance->SetMontage(EMontageType::DEFENSE, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaLancer/ArcdevaLancerDefenseMontage.ArcdevaLancerDefenseMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArcdevaLancer::OnMontageEnded);
	TTAnimInstance->OnDefenseStart.AddUObject(this, &ATTArcdevaLancer::DefenseStart);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArcdevaLancer::AttackCheck);
}

void ATTArcdevaLancer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArcdevaLancer.BT_ArcdevaLancer"));
}

void ATTArcdevaLancer::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArcdevaLancer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArcdevaLancer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	if (!TTAnimInstance->GetCurrentActiveMontage())
	{
		FVector LaunchVector{ GetActorLocation() - DamageCauser->GetActorLocation() };
		float ForceAmount{ 1300.0f };

		TurnToTarget(LastDamageInstigator, 100.0f);
		TTAnimInstance->SetDamaged();
		GetCharacterMovement()->Launch(LaunchVector.GetSafeNormal2D() * ForceAmount);
	}
	else
	{
		if (GetCurrentMontage()->GetName() == TEXT("ArcdevaLancerDefenseMontage"))
			Audio->PlaySoundAtLocation(TEXT("ShieldDefense"), GetActorLocation());
	}

	return FinalDamage;
}

void ATTArcdevaLancer::DefenseStart()
{
	Audio->PlaySoundAtLocation(TEXT("ShieldDefenseStart"), GetActorLocation());
}

void ATTArcdevaLancer::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	if (GetCurrentMontage()->GetName() == TEXT("ArcdevaLancerAttackMontage")) AttackLength = 650.0f;
	else AttackLength = 300.0f;
	AttackRadius = 100.0f;

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
			if (GetCurrentMontage()->GetName() == TEXT("ArcdevaLancerAttackMontage"))
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 7.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
			else
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 10.0f);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 10.0f);
				Audio->PlaySound2D(TEXT("HitChargeAttack"));
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

void ATTArcdevaLancer::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArcdevaLancerAttackMontage")):
	case FTTWorld::HashCode(TEXT("ArcdevaLancerChargeAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	case FTTWorld::HashCode(TEXT("ArcdevaLancerDefenseMontage")):
		OnDefenseEnded.Broadcast();
		break;
	}
}