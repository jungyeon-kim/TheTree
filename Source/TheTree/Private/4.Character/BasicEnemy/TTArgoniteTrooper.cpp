#include "TTArgoniteTrooper.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArgoniteTrooper::ATTArgoniteTrooper()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArgoniteTrooper/SK_Argonite_Trooper.SK_Argonite_Trooper") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteTrooper/ArgoniteTrooperAnimBlueprint.ArgoniteTrooperAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaWarrior_HitImpact.P_ArcdevaWarrior_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArgoniteTrooper/ArgoniteTrooper_Attack_SoundCue.ArgoniteTrooper_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArgoniteTrooper/ArgoniteTrooper_HitAttack_SoundCue.ArgoniteTrooper_HitAttack_SoundCue"));

	GeneralMoveSpeed = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 10.0f;
}

void ATTArgoniteTrooper::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("ArgoniteTrooper"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("HitReact"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteTrooper/ArgoniteTrooperHitReactMontage.ArgoniteTrooperHitReactMontage"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteTrooper/ArgoniteTrooperAttackMontage.ArgoniteTrooperAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArgoniteTrooper::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArgoniteTrooper::AttackCheck);
}

void ATTArgoniteTrooper::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArgoniteTrooper.BT_ArgoniteTrooper"));
}

void ATTArgoniteTrooper::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArgoniteTrooper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArgoniteTrooper::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ATTArgoniteTrooper::AttackCheck()
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
			HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
			Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 1.5f);
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

void ATTArgoniteTrooper::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArgoniteTrooperAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}