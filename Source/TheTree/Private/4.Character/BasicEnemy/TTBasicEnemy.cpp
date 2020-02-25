#include "TTBasicEnemy.h"
#include "TTBasicEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTAudioComponent.h"
#include "DrawDebugHelpers.h"

ATTBasicEnemy::ATTBasicEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));

	RootComponent = GetCapsuleComponent();
	Audio->SetupAttachment(RootComponent);
	GetMesh()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/Arcdeva_Warrior/SK_Arcdeva_Warrior.SK_Arcdeva_Warrior") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaAnimBlueprint.ArcdevaAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GeneralMoveSpeed = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;

	SetCharacterState(ECharacterState::LOADING);
}

void ATTBasicEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TTAnimInstance = Cast<UTTBasicEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BasicEnemy/ArcdevaAttackMontage.ArcdevaAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTBasicEnemy::OnAttackMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTBasicEnemy::AttackCheck);
}

void ATTBasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	TTAIController = Cast<ATTAIController>(GetController());
	TTAIController->SetBehaviorTree(EAIType::BASIC, TEXT("/Game/Blueprints/AI/BT_Basic.BT_Basic"));

	SetCharacterState(ECharacterState::READY);
}

void ATTBasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTBasicEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

float ATTBasicEnemy::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ATTBasicEnemy::Attack()
{
	TTAnimInstance->PlayAttackMontange();
}

void ATTBasicEnemy::AttackCheck()
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

#if ENABLE_DRAW_DEBUG
	FVector Trace{ GetActorForwardVector() * AttackLength };
	FVector Center{ GetActorLocation() + Trace * 0.5f };
	float HalfHeight{ AttackLength * 0.5f + AttackRadius };
	FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
	FColor DrawColor{ bResult ? FColor::Blue : FColor::Red };
	float DebugLifeTime{ 5.0f };
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif
}

void ATTBasicEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(Montage->GetName() == TEXT("ArcdevaAttackMontage")) OnAttackEnded.Broadcast();
}

ECharacterState ATTBasicEnemy::GetCharacterState() const
{
	return CurrentState;
}

void ATTBasicEnemy::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		SetActorHiddenInGame(true);
		bCanBeDamaged = false;

		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		bCanBeDamaged = true;
		TTAIController->RunAI();

		SetCharacterState(ECharacterState::NOBATTLE);
		break;
	}
	case ECharacterState::NOBATTLE:
	{
		GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
		break;
	}
	case ECharacterState::BATTLE:
	{
		GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed * 0.8f;
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		bCanBeDamaged = false;
		TTAIController->StopAI();

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
		{
			Destroy();
		}
		), DeadTimer, false);
		break;
	}
	}
}