#include "TTEnemyBase.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "TTBaseLevel.h"
#include "DrawDebugHelpers.h"

ATTEnemyBase::ATTEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ATTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CameraShake = UTTCameraShake::StaticClass();
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	SetCharacterState(ECharacterState::LOADING);
}

void ATTEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->OnHPIsZero.AddLambda([&]() { SetCharacterState(ECharacterState::DEAD); });
	
	TTAnimInstance = Cast<UTTEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
}

void ATTEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	TTAIController = Cast<ATTAIController>(GetController());
	TTCHECK(TTAIController);

	SetCharacterState(ECharacterState::READY);
}

void ATTEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterState(ECharacterState::READY);
}

void ATTEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTEnemyBase::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };

	LastDamageInstigator = DamageCauser;
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ATTEnemyBase::TurnToTarget(AActor* Target, float InterpSpeed)
{
	FVector DirectionToTarget{ Target->GetActorLocation() - GetActorLocation() };
	FRotator TargetRot{ FRotationMatrix::MakeFromX(DirectionToTarget.GetSafeNormal2D()).Rotator() };

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), InterpSpeed));
}

void ATTEnemyBase::PlayMontage(FName MontageType)
{
	TTAnimInstance->PlayMontage(MontageType);
}

ECharacterState ATTEnemyBase::GetCharacterState() const
{
	return CurrentState;
}

void ATTEnemyBase::SetPlayRate(float StartTime, float EndTime, float TimeDilation)
{
	FTimerHandle DelayTimerHandle[2]{};
	FTTWorld::TimeDilation = TimeDilation;

	if (UGameplayStatics::GetGlobalTimeDilation(this) == 1.0f)
	{
		if (!StartTime) UGameplayStatics::SetGlobalTimeDilation(this, FTTWorld::TimeDilation);
		else GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle[0], FTimerDelegate::CreateLambda(
			[&]() { UGameplayStatics::SetGlobalTimeDilation(this, FTTWorld::TimeDilation); }), StartTime, false);
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle[1], FTimerDelegate::CreateLambda(
			[&]() { UGameplayStatics::SetGlobalTimeDilation(this, 1.0f); }), EndTime, false);
	}
}

void ATTEnemyBase::SetCharacterState(ECharacterState NewState)
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
		//TTAIController->RunAI();
		
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

		FTimerHandle DeadTimerHandle{};
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{ 
				ATTBaseLevel* CurrentLevel{ Cast<ATTBaseLevel>(GetWorld()->GetLevelScriptActor()) };
				if (CurrentLevel)
					CurrentLevel->AddMonsterCount(-1);
				Destroy(); 
			
			}), DeadTimer, false);
		

		
		break;
	}
	}
}
