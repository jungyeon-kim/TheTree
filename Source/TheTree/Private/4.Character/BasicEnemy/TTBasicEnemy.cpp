#include "TTBasicEnemy.h"
#include "TTBasicEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTBasicEnemy::ATTBasicEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Effect = CreateDefaultSubobject<UTTParticleSystemComponent>(TEXT("EFFECT"));
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));
	CharacterStat = CreateDefaultSubobject<UTTCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	RootComponent = GetCapsuleComponent();
	Effect->SetupAttachment(RootComponent);
	Audio->SetupAttachment(RootComponent);
	GetMesh()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };
	DeadTimer = 20.0f;

	SetCharacterState(ECharacterState::LOADING);
}

void ATTBasicEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->OnHPIsZero.AddLambda([&]()
	{
		SetCharacterState(ECharacterState::DEAD);
	});

	TTAnimInstance = Cast<UTTBasicEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
}

void ATTBasicEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController = Cast<ATTAIController>(GetController());
	TTCHECK(TTAIController);
}

void ATTBasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterState(ECharacterState::READY);
}

void ATTBasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTBasicEnemy::TakeDamage(float DamageAmount, const FDamageEvent & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };

	LastDamageInstigator = DamageCauser;
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ATTBasicEnemy::TurnToTarget(AActor* Target, float InterpSpeed)
{
	FVector DirectionToTarget{ Target->GetActorLocation() - GetActorLocation() };
	FRotator TargetRot{ FRotationMatrix::MakeFromX(DirectionToTarget.GetSafeNormal2D()).Rotator() };

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), InterpSpeed));
}

void ATTBasicEnemy::Attack()
{
	TTAnimInstance->PlayAttackMontange();
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
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		TTAIController->StopAI();
		TTAnimInstance->StopAllMontages(0.25f);
		TTAnimInstance->SetDead();

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