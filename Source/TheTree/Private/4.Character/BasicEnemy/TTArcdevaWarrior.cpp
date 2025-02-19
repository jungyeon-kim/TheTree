#include "TTArcdevaWarrior.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTAIStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArcdevaWarrior::ATTArcdevaWarrior()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArcdevaWarrior/SK_Arcdeva_Warrior.SK_Arcdeva_Warrior") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaWarrior/ArcdevaWarriorAnimBlueprint.ArcdevaWarriorAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaWarrior_HitImpact.P_ArcdevaWarrior_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaWarrior/ArcdevaWarrior_Attack_SoundCue.ArcdevaWarrior_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaWarrior/ArcdevaWarrior_HitAttack_SoundCue.ArcdevaWarrior_HitAttack_SoundCue"));

	GetCapsuleComponent()->SetCapsuleSize(88.0f, 88.0f);
	GeneralMoveSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 10.0f;
}

void ATTArcdevaWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIStat->SetObjectStat(TEXT("ArcdevaWarrior"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("HitReact"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaWarrior/ArcdevaWarriorHitReactMontage.ArcdevaWarriorHitReactMontage"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaWarrior/ArcdevaWarriorAttackMontage.ArcdevaWarriorAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArcdevaWarrior::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArcdevaWarrior::AttackCheck);
}

void ATTArcdevaWarrior::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArcdevaWarrior.BT_ArcdevaWarrior"));
}

void ATTArcdevaWarrior::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArcdevaWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArcdevaWarrior::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - AIStat->GetDef() / 100.0f));

	if (GetVelocity().IsNearlyZero() && (!TTAnimInstance->GetCurrentActiveMontage() || DamageEvent.GetTypeID() == 1))
	{
		FVector LaunchVector{ GetActorLocation() - DamageCauser->GetActorLocation() };
		float ForceAmount{ 1300.0f };

		TurnToTarget(LastDamageInstigator, 100.0f);
		GetCharacterMovement()->Launch(LaunchVector.GetSafeNormal2D() * ForceAmount);

		TTAnimInstance->PlayMontage(TEXT("HitReact"));
	}

	return FinalDamage;
}

void ATTArcdevaWarrior::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	AttackLength = 150.0f;
	AttackRadius = 50.0f;
	HitStartLocation = GetActorForwardVector() * AttackRadius;

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

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(AIStat->GetAtk(), DamageEvent, GetController(), this);
			Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 2.5f);
			Audio->PlaySoundCue2D(TEXT("HitAttack"));
		}
	Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());

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

void ATTArcdevaWarrior::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArcdevaWarriorAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}