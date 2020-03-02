#include "TTArcdevaWarrior.h"
#include "TTBasicEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTAudioComponent.h"
#include "DrawDebugHelpers.h"

ATTArcdevaWarrior::ATTArcdevaWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArcdevaWarrior/SK_Arcdeva_Warrior.SK_Arcdeva_Warrior") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaWarrior/ArcdevaAnimBlueprint.ArcdevaAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	GeneralMoveSpeed = 600.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
}

void ATTArcdevaWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaWarrior/ArcdevaAttackMontage.ArcdevaAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArcdevaWarrior::OnAttackMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArcdevaWarrior::AttackCheck);
}

void ATTArcdevaWarrior::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(EAIType::BASIC, TEXT("/Game/Blueprints/AI/BT_Basic.BT_Basic"));
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
	if (!TTAnimInstance->GetCurrentActiveMontage())
	{
		FVector LaunchVector{ GetActorLocation() - DamageCauser->GetActorLocation() };
		float ForceAmount{ 1300.0f };

		TTAnimInstance->SetHitReactAnim();
		GetCharacterMovement()->Launch(LaunchVector.GetSafeNormal2D() * ForceAmount);
	}

	return 0.0f;
}

void ATTArcdevaWarrior::AttackCheck()
{
	AttackLength = 200.0f;
	AttackRadius = 50.0f;

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
			TTLOG(Warning, TEXT("Hit Actor Name: %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
		}

	if (FTTDebug::bIsDebugging)
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

void ATTArcdevaWarrior::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetName() == TEXT("ArcdevaAttackMontage")) OnAttackEnded.Broadcast();
}