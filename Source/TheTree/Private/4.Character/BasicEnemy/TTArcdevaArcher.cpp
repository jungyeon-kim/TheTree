#include "TTArcdevaArcher.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArcdevaArcher::ATTArcdevaArcher()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArcdevaArcher/SK_Arcdeva_Archer.SK_Arcdeva_Archer") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherAnimBlueprint.ArcdevaArcherAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("Shot"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaArcher_Shot.P_ArcdevaArcher_Shot"));
	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaArcher_HitImpact.P_ArcdevaArcher_HitImpact"));
	Audio->AddSoundWave(TEXT("AttackStart"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaArcher/ArcdevaArcher_AttackStart.ArcdevaArcher_AttackStart"));
	Audio->AddSoundWave(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaArcher/ArcdevaArcher_HitAttack.ArcdevaArcher_HitAttack"));

	GeneralMoveSpeed = 800.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 10.0f;
}

void ATTArcdevaArcher::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("ArcdevaArcher"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("HitReact"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherHitReactMontage.ArcdevaArcherHitReactMontage"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherAttackMontage.ArcdevaArcherAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("ChargeAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherChargeAttackMontage.ArcdevaArcherChargeAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArcdevaArcher::OnMontageEnded);
	TTAnimInstance->OnStartInit.AddUObject(this, &ATTArcdevaArcher::StartInit);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArcdevaArcher::AttackCheck);
}

void ATTArcdevaArcher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArcdevaArcher.BT_ArcdevaArcher"));
}

void ATTArcdevaArcher::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArcdevaArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArcdevaArcher::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

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

void ATTArcdevaArcher::StartInit()
{
	AttackStartForwardVector = GetActorForwardVector();
	Effect->PlayEffectAtLocation(TEXT("Shot"), GetActorLocation(), GetActorForwardVector().Rotation(), FVector(9.0f, 2.0f, 2.0f));
	Audio->PlaySoundWaveAtLocation(TEXT("AttackStart"), GetActorLocation());
}

void ATTArcdevaArcher::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	AttackLength = 2980.0f;
	AttackRadius = 20.0f;
	HitStartLocation = GetActorForwardVector() * AttackRadius;

	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + HitStartLocation,
		GetActorLocation() + AttackStartForwardVector * AttackLength + HitStartLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
			Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
				GetActorForwardVector().Rotation(), 2.5f);
			Audio->PlaySoundWave2D(TEXT("HitAttack"));
		}

	if (FTTWorld::bIsDebugging)
	{
		FVector Trace{ AttackStartForwardVector * AttackLength };
		FVector Center{ GetActorLocation() + Trace * 0.5f + HitStartLocation };
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
		FColor DrawColor{ bResult ? FColor::Blue : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTArcdevaArcher::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArcdevaArcherAttackMontage")):
	case FTTWorld::HashCode(TEXT("ArcdevaArcherChargeAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}