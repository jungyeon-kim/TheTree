#include "TTPlayer.h"
#include "TTPlayerController.h"
#include "TTPlayerWeapon.h"
#include "TTPlayerAnimInstance.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "DrawDebugHelpers.h"

ATTPlayer::ATTPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CameraShake = UTTCameraShake::StaticClass();
	Effect = CreateDefaultSubobject<UTTParticleSystemComponent>(TEXT("EFFECT"));
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));
	
	RootComponent = GetCapsuleComponent();
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	Effect->SetupAttachment(RootComponent);
	Audio->SetupAttachment(RootComponent);
	GetMesh()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PLAYER{ TEXT("/Game/Assets/Character/Player/SK_Player.SK_Player") };
	static ConstructorHelpers::FClassFinder<UTTPlayerAnimInstance> PLAYER_ANIM{ TEXT("/Game/Blueprints/Animation/Player/PlayerAnimBlueprint.PlayerAnimBlueprint_C") };
	if (SK_PLAYER.Succeeded()) GetMesh()->SetSkeletalMesh(SK_PLAYER.Object);
	if (PLAYER_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(PLAYER_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_Player_HitImpact.P_Player_HitImpact"));
	Audio->AddSound(TEXT("Attack"), TEXT("/Game/Assets/Sound/Player/Player_Attack_SoundCue.Player_Attack_SoundCue"));
	Audio->AddSound(TEXT("TargetAttack"), TEXT("/Game/Assets/Sound/Player/Player_TargetAttack_SoundCue.Player_TargetAttack_SoundCue"));
	Audio->AddSound(TEXT("AttackVoice"), TEXT("/Game/Assets/Sound/Player/Player_AttackVoice_SoundCue.Player_AttackVoice_SoundCue"));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->TargetArmLength = 800.0f;
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	MaxCombo = 4;
	DeadTimer = 5.0f;
	GeneralMoveSpeed = 1000.0f;
	AdvancedMoveSpeed = GeneralMoveSpeed * 1.2f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = 1100.0f;
	GetCharacterMovement()->GravityScale = 3.0f;

	SetCharacterState(ECharacterState::LOADING);
}

void ATTPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TTAnimInstance = Cast<UTTPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPlayer::OnAttackMontageEnded);
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPlayer::OnDodgeMontageEnded);
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPlayer::OnInOutWeaponMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTPlayer::AttackCheck);
	TTAnimInstance->OnNextAttackCheck.AddLambda([&]()
	{
		bCanNextCombo = false;
		if (bIsComboInputOn)
		{
			AttackStartComboState();
			TTAnimInstance->JumpToAttackMontageSection(CurrentCombo);
		}
		else TTAnimInstance->StopAllMontages(0.25f);
	});
	TTAnimInstance->OnSwapWeapon.AddUObject(this, &ATTPlayer::SetWeapon);
	TTAnimInstance->OnPlaySound.AddLambda([&]()
	{
		Audio->PlaySound2D(TEXT("AttackVoice"));
	});
}

void ATTPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("SwapDebugMode"), EInputEvent::IE_Pressed, this, &ATTPlayer::SwapDebugMode);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATTPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &ATTPlayer::Dodge);
	PlayerInputComponent->BindAction(TEXT("SwapBattleMode"), EInputEvent::IE_Pressed, this, &ATTPlayer::SwapBattleMode);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATTPlayer::Jump);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ATTPlayer::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ATTPlayer::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATTPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATTPlayer::Turn);
}

void ATTPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATTPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	TTPlayerController = Cast<ATTPlayerController>(GetController());
	CurrentWeapon = GetWorld()->SpawnActor<ATTPlayerWeapon>();
	if (CurrentWeapon) CurrentWeapon->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Back_Socket"));

	SetCharacterState(ECharacterState::READY);
}

void ATTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		FVector CameraForwardVector{ Camera->GetForwardVector() };
		CameraForwardVector.Z = 0.0f;
		FRotator TargetRot{ FRotationMatrix::MakeFromX(CameraForwardVector).Rotator() };
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));
	}
}

float ATTPlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ATTPlayer::Attack()
{
	if (bIsAttacking)
	{
		TTCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (bCanNextCombo) bIsComboInputOn = true;
	}
	else if (GetCurrentStateNodeName() == FName("Ground") && !TTAnimInstance->GetCurrentActiveMontage()
		&& CurrentState == ECharacterState::BATTLE)
	{
		TTCHECK(!CurrentCombo);
		AttackStartComboState();
		TTAnimInstance->PlayAttackMontange();
		TTAnimInstance->JumpToAttackMontageSection(CurrentCombo);
		bIsAttacking = true;
	}
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
	if (CurrentCombo == 4)
	{
		AttackLength = 1.0f;
		AttackRadius = 330.0f;
	}
	else
	{
		AttackLength = 300.0f;
		AttackRadius = 100.0f;
	}

	TArray<FOverlapResult> HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	FVector Trace{ GetActorForwardVector() * AttackLength };
	FVector Center{ GetActorLocation() + Trace * 0.75f };
	FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResult,
		Center,
		CapsuleRot,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeCapsule(AttackRadius, AttackLength),
		Params);

	if (bResult)
	{
		for (const auto& Result : HitResult)
			if (Result.Actor.IsValid())
			{
				TTLOG(Warning, TEXT("Hit Actor Name: %s"), *Result.Actor->GetName());

				FDamageEvent DamageEvent{};
				Result.Actor->TakeDamage(20.0f, DamageEvent, GetController(), this);
				Effect->PlayEffect(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 8.0f);
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
		Audio->PlaySound2D(TEXT("TargetAttack"));
	}
	Audio->PlaySound2D(TEXT("Attack"));


	if (FTTDebug::bIsDebugging)
	{
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FColor DrawColor{ bResult ? FColor::Green : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetName() == TEXT("PlayerAttackMontage"))
	{
		bIsAttacking = false;
		AttackEndComboState();
	}
}

void ATTPlayer::OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetName() == TEXT("PlayerDodgeMontage")) bIsDodging = false;
}

void ATTPlayer::OnInOutWeaponMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (Montage->GetName() == TEXT("PlayerInWeaponMontage") || Montage->GetName() == TEXT("PlayerOutWeaponMontage"))
		bIsSwappingWeapon = false;
}

ECharacterState ATTPlayer::GetCharacterState() const
{
	return CurrentState;
}

FName ATTPlayer::GetCurrentStateNodeName() const
{
	return TTAnimInstance->GetCurrentStateName(TTAnimInstance->GetStateMachineIndex(FName("BaseAction")));
}

void ATTPlayer::SetWeapon()
{
	TTCHECK(CurrentWeapon);

	if (TTAnimInstance->GetIsBattleOn()) CurrentWeapon->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("rHand_Socket"));
	else CurrentWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Back_Socket"));
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
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = { 0.0f, 720.0f, 0.0f };
		break;
	}
}

void ATTPlayer::Dodge()
{
	if (!bIsDodging && !bIsSwappingWeapon)
	{
		TTAnimInstance->PlayDodgeMontage();
		bIsDodging = true;
	}
}

void ATTPlayer::SwapBattleMode()
{
	if (GetCurrentStateNodeName() == TEXT("Ground") && !TTAnimInstance->GetCurrentActiveMontage())
	{
		TTAnimInstance->PlayInOutWeaponMontage();
		if (TTAnimInstance->GetIsBattleOn()) SetCharacterState(ECharacterState::BATTLE);
		else SetCharacterState(ECharacterState::NOBATTLE);
		bIsSwappingWeapon = true;
	}
}

void ATTPlayer::Jump()
{
	if (GetCurrentStateNodeName() == TEXT("Ground") && !TTAnimInstance->GetCurrentActiveMontage()
		&& CurrentState == ECharacterState::NOBATTLE)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ATTPlayer::UpDown(float NewAxisValue)
{
	if (GetCurrentStateNodeName() == TEXT("Ground") && !TTAnimInstance->GetCurrentActiveMontage())
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void ATTPlayer::LeftRight(float NewAxisValue)
{
	if (GetCurrentStateNodeName() == TEXT("Ground") && !TTAnimInstance->GetCurrentActiveMontage())
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

void ATTPlayer::SwapDebugMode()
{
	if (!FTTDebug::bIsDebugging) FTTDebug::bIsDebugging = true;
	else FTTDebug::bIsDebugging = false;
}