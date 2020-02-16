#include "TTPlayer.h"
#include "TTPlayerController.h"
#include "TTAnimInstance.h"

ATTPlayer::ATTPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = GetCapsuleComponent();
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PLAYER{ TEXT("/Game/Assets/Characters/Player/SK_Player.SK_Player") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> PLAYER_ANIM{ TEXT("/Game/Blueprints/Animations/PlayerAnimBlueprint.PlayerAnimBlueprint_C") };
	if (SK_PLAYER.Succeeded()) GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	if (PLAYER_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 1100.0f;
	GetCharacterMovement()->GravityScale = 3.0f;
	SpringArm->TargetArmLength = 800.0f;
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	MaxCombo = 4;
	AttackLength = 200.0f;
	AttackRadius = 50.0f;
	DeadTimer = 5.0f;

	SetCharacterState(ECharacterState::LOADING);
}

void ATTPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TTAnimInstance = Cast<UTTAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPlayer::OnAttackMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTPlayer::AttackCheck);
	TTAnimInstance->OnNextAttackCheck.AddLambda([&]()
	{
		bCanNextCombo = false;
		if (bIsComboInputOn)
		{
			AttackStartComboState();
			TTAnimInstance->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

void ATTPlayer::BeginPlay()
{
	Super::BeginPlay();

	TTPlayerController = Cast<ATTPlayerController>(GetController());

	SetCharacterState(ECharacterState::READY);
}

void ATTPlayer::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::THIRD_PERSON:
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;								// 컨트롤러의 Yaw회전을 캐릭터에 적용
		GetCharacterMovement()->bOrientRotationToMovement = true;		// 움직이는 방향으로 회전
		GetCharacterMovement()->bUseControllerDesiredRotation = false;	// RotationRate에 따라 부드럽게 회전
		GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };	// 초당 회전량
		break;
	}
}

void ATTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATTPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("BasicAttack"), EInputEvent::IE_Pressed, this, &ATTPlayer::BasicAttack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ATTPlayer::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ATTPlayer::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATTPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATTPlayer::Turn);
}

float ATTPlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ATTPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATTPlayer::BasicAttack()
{
}

bool ATTPlayer::CanSetWeapon()
{
	return false;
}

void ATTPlayer::SetWeapon(AABWeapon* NewWeapon)
{
}

ECharacterState ATTPlayer::GetCharacterState() const
{
	return CurrentState;
}

void ATTPlayer::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		SetActorHiddenInGame(true);
		bCanBeDamaged = false;

		DisableInput(TTPlayerController);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		bCanBeDamaged = true;

		SetControlMode(EControlMode::THIRD_PERSON);
		EnableInput(TTPlayerController);
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		bCanBeDamaged = false;

		DisableInput(TTPlayerController);

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
			[&]()
		{
			TTPlayerController->RestartLevel();
		}
		), DeadTimer, false);
		break;
	}
	}
}

FName ATTPlayer::GetCurrentStateMachineName() const
{
	return TTAnimInstance->GetCurrentStateName(TTAnimInstance->GetStateMachineIndex(FName("BaseAction")));
}

void ATTPlayer::UpDown(float NewAxisValue)
{
	if (GetCurrentStateMachineName() == FName("Ground") && !TTAnimInstance->IsAnyMontagePlaying())
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ATTPlayer::LeftRight(float NewAxisValue)
{
	if (GetCurrentStateMachineName() == FName("Ground") && !TTAnimInstance->IsAnyMontagePlaying())
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void ATTPlayer::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ATTPlayer::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void ATTPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

void ATTPlayer::AttackStartComboState()
{
}

void ATTPlayer::AttackEndComboState()
{
}

void ATTPlayer::AttackCheck()
{
}
