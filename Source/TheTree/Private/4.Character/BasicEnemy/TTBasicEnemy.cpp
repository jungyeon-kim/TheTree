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
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };

	SetCharacterState(ECharacterState::LOADING);
}

void ATTBasicEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

float ATTBasicEnemy::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
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