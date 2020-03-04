#include "TTArcdevaArcher.h"
#include "TTBasicEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArcdevaArcher::ATTArcdevaArcher()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArcdevaArcher/SK_Arcdeva_Archer.SK_Arcdeva_Archer") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherAnimBlueprint.ArcdevaArcherAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("Shot"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaArcher_Shot.P_ArcdevaArcher_Shot"));
	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaArcher_HitImpact.P_ArcdevaArcher_HitImpact"));
	Audio->AddSound(TEXT("AttackStart"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaArcher/Arcdeva_Archer_AttackStart.Arcdeva_Archer_AttackStart"));
	Audio->AddSound(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaArcher/Arcdeva_Archer_HitAttack.Arcdeva_Archer_HitAttack"));

	GeneralMoveSpeed = 800.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
}

void ATTArcdevaArcher::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(FName("ArcdevaArcher"));

	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaArcher/ArcdevaArcherAttackMontage.ArcdevaArcherAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArcdevaArcher::OnAttackMontageEnded);
	TTAnimInstance->OnAttackStart.AddUObject(this, &ATTArcdevaArcher::AttackStart);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArcdevaArcher::AttackCheck);
}

void ATTArcdevaArcher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(EAIType::BASIC, TEXT("/Game/Blueprints/AI/BT_ArcdevaArcher.BT_ArcdevaArcher"));
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

	if (!TTAnimInstance->GetCurrentActiveMontage())
	{
		FVector LaunchVector{ GetActorLocation() - DamageCauser->GetActorLocation() };
		float ForceAmount{ 1300.0f };

		TurnToTarget(LastDamageInstigator, 100.0f);
		TTAnimInstance->SetDamaged();
		GetCharacterMovement()->Launch(LaunchVector.GetSafeNormal2D() * ForceAmount);
	}

	return FinalDamage;
}

void ATTArcdevaArcher::AttackStart()
{
	AttackStartForwardVector = GetActorForwardVector();
	Effect->PlayEffect(TEXT("Shot"), GetActorLocation(), GetActorForwardVector().Rotation(), FVector(9.0f, 2.0f, 2.0f));
	Audio->PlaySoundAtLocation(TEXT("AttackStart"), GetActorLocation());
}

void ATTArcdevaArcher::AttackCheck()
{
	AttackLength = 3000.0f;
	AttackRadius = 20.0f;

	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + AttackStartForwardVector * AttackLength ,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
			Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 2.5f);
			Audio->PlaySound2D(TEXT("HitAttack"));
		}

	if (FTTWorld::bIsDebugging)
	{
		FVector Trace{ AttackStartForwardVector * AttackLength };
		FVector Center{ GetActorLocation() + Trace * 0.5f };
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
		FColor DrawColor{ bResult ? FColor::Blue : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTArcdevaArcher::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetName() == TEXT("ArcdevaArcherAttackMontage")) OnAttackEnded.Broadcast();
}