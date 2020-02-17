#include "TTPlayer.h"
#include "TTPlayerController.h"
#include "TTAnimInstance.h"
#include "DrawDebugHelpers.h"

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
	AttackLength = 300.0f;
	AttackRadius = 100.0f;
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
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATTPlayer::Attack);

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

void ATTPlayer::Attack()
{
	if (GetCurrentStateMachineName() == FName("Ground"))
	{
		if (bIsAttacking)
		{
			TTCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
			if (bCanNextCombo) bIsComboInputOn = true;
		}
		else
		{
			TTCHECK(!CurrentCombo);
			AttackStartComboState();
			TTAnimInstance->PlayAttackMontange();
			TTAnimInstance->JumpToAttackMontageSection(CurrentCombo);
			bIsAttacking = true;
		}
	}
}

void ATTPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TTCHECK(bIsAttacking);
	TTCHECK(CurrentCombo > 0);
	bIsAttacking = false;
	AttackEndComboState();
	OnAttackEnded.Broadcast();
}

void ATTPlayer::AttackStartComboState()
{
	bCanNextCombo = true;
	bIsComboInputOn = false;
	TTCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ATTPlayer::AttackEndComboState()
{
	bIsComboInputOn = false;
	bCanNextCombo = false;
	CurrentCombo = 0;
}

void ATTPlayer::AttackCheck()
{
	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackLength,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// Attack 트레이스 채널
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector Trace{ GetActorForwardVector() * AttackLength };
	FVector Center{ GetActorLocation() + Trace * 0.5f };
	float HalfHeight{ AttackLength * 0.5f + AttackRadius };
	FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
	FColor DrawColor{ bResult ? FColor::Green : FColor::Red };
	float DebugLifeTime{ 5.0f };
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
#endif

	if (bResult)
		if (HitResult.Actor.IsValid())
		{
			TTLOG(Warning, TEXT("Hit Actor Name: %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent{};
			HitResult.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
		}
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

FName ATTPlayer::GetCurrentStateMachineName() const
{
	return TTAnimInstance->GetCurrentStateName(TTAnimInstance->GetStateMachineIndex(FName("BaseAction")));
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

void ATTPlayer::Jump()
{
	if (GetCurrentStateMachineName() == FName("Ground") && !TTAnimInstance->IsAnyMontagePlaying())
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
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