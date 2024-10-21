#include "TTArgoniteGuardian.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTAIStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

ATTArgoniteGuardian::ATTArgoniteGuardian()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArgoniteGuardian/SK_Argonite_Guardian.SK_Argonite_Guardian") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGuardian/ArgoniteGuardianAnimBlueprint.ArgoniteGuardianAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaArcher_HitImpact.P_ArcdevaArcher_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArgoniteGuardian/ArgoniteGuardian_Attack_SoundCue.ArgoniteGuardian_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitAttack_SoundCue.ArcdevaLancer_HitAttack_SoundCue"));
	Audio->AddSoundWave(TEXT("ShieldDefense"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_ShieldDefence.ArcdevaLancer_ShieldDefence"));

	GetCapsuleComponent()->SetCapsuleSize(150.0f, 150.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -150.0f));
	GeneralMoveSpeed = 700.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 10.0f;
}

void ATTArgoniteGuardian::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIStat->SetObjectStat(TEXT("ArcdevaLancer"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("HitReact"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGuardian/ArgoniteGuardianHitReactMontage.ArgoniteGuardianHitReactMontage"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGuardian/ArgoniteGuardianAttackMontage.ArgoniteGuardianAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("ChargeAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGuardian/ArgoniteGuardianChargeAttackMontage.ArgoniteGuardianChargeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("Defense"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGuardian/ArgoniteGuardianDefenseMontage.ArgoniteGuardianDefenseMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArgoniteGuardian::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArgoniteGuardian::AttackCheck);
}

void ATTArgoniteGuardian::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArgoniteGuardian.BT_ArgoniteGuardian"));
}

void ATTArgoniteGuardian::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArgoniteGuardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArgoniteGuardian::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - AIStat->GetDef() / 100.0f));

	if (GetCurrentMontage())
	{
		if (GetCurrentMontage()->GetName() == TEXT("ArgoniteGuardianDefenseMontage"))
			Audio->PlaySoundWaveAtLocation(TEXT("ShieldDefense"), GetActorLocation());
	}

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

void ATTArgoniteGuardian::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	if (GetCurrentMontage()->GetName() == TEXT("ArgoniteGuardianAttackMontage")) AttackLength = 300.0f;
	else AttackLength = 300.0f;
	AttackRadius = 100.0f;
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
		if (HitResult.GetActor())
		{
			if (GetCurrentMontage()->GetName() == TEXT("ArgoniteGuardianAttackMontage"))
			{
				FDamageEvent DamageEvent{};
				HitResult.GetActor()->TakeDamage(AIStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
			else
			{
				FDamageEvent DamageEvent{};
				HitResult.GetActor()->TakeDamage(AIStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
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

void ATTArgoniteGuardian::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArgoniteGuardianAttackMontage")):
	case FTTWorld::HashCode(TEXT("ArgoniteGuardianChargeAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	case FTTWorld::HashCode(TEXT("ArgoniteGuardianDefenseMontage")):
		OnDefenseEnded.Broadcast();
		break;
	}
}