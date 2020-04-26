#include "TTPlayer.h"
#include "TTPlayerController.h"
#include "TTPlayerWeapon.h"
#include "TTPlayerAnimInstance.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "TTGhostTrail.h"
#include "DrawDebugHelpers.h"

ATTPlayer::ATTPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CameraShake = UTTCameraShake::StaticClass();
	Effect = CreateDefaultSubobject<UTTParticleSystemComponent>(TEXT("EFFECT"));
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));
	CharacterStat = CreateDefaultSubobject<UTTCharacterStatComponent>(TEXT("CHARACTERSTAT"));

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
	Effect->AddEffect(TEXT("Lightning"), TEXT("/Game/Assets/Effect/Particle/P_Player_Lightning.P_Player_Lightning"));
	Effect->AddEffect(TEXT("GaiaLightning"), TEXT("/Game/Assets/Effect/Particle/P_Player_GaiaLightning.P_Player_GaiaLightning"));
	Effect->AddEffect(TEXT("GaiaImpact"), TEXT("/Game/Assets/Effect/Particle/P_Player_GaiaImpact.P_Player_GaiaImpact"));
	Effect->AddEffect(TEXT("SwordAttach1"), TEXT("/Game/Assets/Effect/Particle/P_Player_SwordAttach_00.P_Player_SwordAttach_00"));
	Effect->AddEffect(TEXT("SwordAttach2"), TEXT("/Game/Assets/Effect/Particle/P_Player_SwordAttach_01.P_Player_SwordAttach_01"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/Player/Player_Attack_SoundCue.Player_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/Player/Player_HitAttack_SoundCue.Player_HitAttack_SoundCue"));
	Audio->AddSoundCue(TEXT("SlidingSlash"), TEXT("/Game/Assets/Sound/Player/Player_SlidingSlash_Shot_SoundCue.Player_SlidingSlash_Shot_SoundCue"));
	Audio->AddSoundWave(TEXT("HitSmashAttack"), TEXT("/Game/Assets/Sound/Player/Player_SmashAttack_Shot.Player_SmashAttack_Shot"));
	Audio->AddSoundWave(TEXT("HitSlidingSlash"), TEXT("/Game/Assets/Sound/Player/Player_SlidingSlash_Hit.Player_SlidingSlash_Hit"));
	Audio->AddSoundWave(TEXT("HitWindCutter"), TEXT("/Game/Assets/Sound/Player/Player_WindCutter_Shot.Player_WindCutter_Shot"));
	Audio->AddSoundWave(TEXT("HitDrawSword"), TEXT("/Game/Assets/Sound/Player/Player_DrawSword_Hit.Player_DrawSword_Hit"));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->TargetArmLength = 800.0f;
	MaxCombo = 4;
	DeadTimer = 5.0f;
	GeneralMoveSpeed = 1000.0f;
	AdvancedMoveSpeed = GeneralMoveSpeed * 1.2f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	GetCharacterMovement()->JumpZVelocity = 1100.0f;
	GetCharacterMovement()->GravityScale = 3.0f;
	for (int i = 0; i < 5; ++i) bIsSkillAttacking.Emplace(false);

	SetCharacterState(ECharacterState::LOADING);
}

void ATTPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("Player"));
	CharacterStat->OnHPIsZero.AddLambda([&]()
		{
			SetCharacterState(ECharacterState::DEAD);
		});

	TTAnimInstance = Cast<UTTPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	TTCHECK(TTAnimInstance);
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTPlayer::OnMontageEnded);
	TTAnimInstance->OnStartInit.AddUObject(this, &ATTPlayer::StartInit);
	TTAnimInstance->OnEndInit.AddUObject(this, &ATTPlayer::EndInit);
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
}

void ATTPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATTPlayer::Attack);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("SmashAttack"), EInputEvent::IE_Pressed, this, &ATTPlayer::SkillAttack, 0);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("SkillAttack1"), EInputEvent::IE_Pressed, this, &ATTPlayer::SkillAttack, 1);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("SkillAttack2"), EInputEvent::IE_Pressed, this, &ATTPlayer::SkillAttack, 2);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("SkillAttack3"), EInputEvent::IE_Pressed, this, &ATTPlayer::SkillAttack, 3);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("SkillAttack4"), EInputEvent::IE_Pressed, this, &ATTPlayer::SkillAttack, 4);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &ATTPlayer::Dodge, 1);
	PlayerInputComponent->BindAction<TBaseDelegate<void, int32>>(TEXT("BackMove"), EInputEvent::IE_Pressed, this, &ATTPlayer::Dodge, 2);
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
	
	if (bIsAttacking || bIsSkillAttacking.Find(true) > -1)
	{
		FVector CameraForwardVector{ Camera->GetForwardVector().GetSafeNormal2D() };
		FRotator TargetRot{ FRotationMatrix::MakeFromX(CameraForwardVector).Rotator() };
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 10.0f));
	}
}

float ATTPlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	LastDamageInstigator = DamageCauser;

	if (!bIsKnockBacking && DamageEvent.GetTypeID() == 1)	// 1(FPointDamageEvent) is critical damage type.
	{
		if (CurrentState == ECharacterState::NOBATTLE)
		{
			TTAnimInstance->SetIsBattleOn(true);
			SetWeapon();
			SetCharacterState(ECharacterState::BATTLE);
		}
		TTAnimInstance->StopAllMontages(0.25f);
		TurnToTarget(LastDamageInstigator, 100.0f);
		TTAnimInstance->PlayMontage(TEXT("KnockBack"));
		bIsKnockBacking = true;
	}

	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ATTPlayer::StartInit()
{
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PlayerDodgeMontage")):
	case FTTWorld::HashCode(TEXT("PlayerBackMoveMontage")):
		PlayGhostTrail(GetMesh(), 0.05f, 1.0f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerSlidingSlashAttackMontage")):
	{
		const auto& SwordAttach2{ Effect->PlayEffectAttached(TEXT("SwordAttach2"), RootComponent, FVector::ZeroVector, 4.0f) };
		Effect->AddManagedEffect(TEXT("SlidingSlash_SwordAttach2"), SwordAttach2);
		PlayGhostTrail(GetMesh(), 0.05f, 1.0f);
		break;
	}
	case FTTWorld::HashCode(TEXT("PlayerWindCutterAttackMontage")):
	{
		const auto& SwordAttach1{ Effect->PlayEffectAttached(TEXT("SwordAttach1"), CurrentWeapon->GetRootComponent(),
			FVector::ZeroVector, FVector(4.0f, 4.0f, 1.0f)) };
		const auto& SwordAttach2{ Effect->PlayEffectAttached(TEXT("SwordAttach2"), CurrentWeapon->GetRootComponent(),
			FVector(0.0f, 0.0f, 100.0f), 0.7f) };
		Effect->AddManagedEffect(TEXT("WindCutter_SwordAttach1"), SwordAttach1);
		Effect->AddManagedEffect(TEXT("WindCutter_SwordAttach2"), SwordAttach2);
		break;
	}
	case FTTWorld::HashCode(TEXT("PlayerGaiaCrushAttackMontage")):
		Effect->PlayEffectAttached(TEXT("Lightning"), RootComponent, FVector(0.0f, 0.0f, 250.0f), 5.0f);
		SetPlayRate(0.5f, 0.57f, 0.1f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerDrawSwordAttackMontage")):
	{
		FTimerHandle TimerHandle[2]{};

		CurrentWeapon->SetActorRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle[0], FTimerDelegate::CreateLambda(
			[&]() { if (CurrentWeapon->GetActorRelativeScale3D().Size() != FMath::Sqrt(3))
			CurrentWeapon->SetActorRelativeScale3D(FVector(2.0f, 2.0f, 2.0f)); }), 0.3f, false);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle[1], FTimerDelegate::CreateLambda(
			[&]() { if (CurrentWeapon->GetActorRelativeScale3D().Size() != FMath::Sqrt(3))
			CurrentWeapon->SetActorRelativeScale3D(FVector(2.5f, 2.5f, 2.5f)); }), 0.6f, false);

		Effect->PlayEffectAtLocation(TEXT("Lightning"), GetActorLocation(), 10.0f);
		const auto& SwordAttach1{ Effect->PlayEffectAttached(TEXT("SwordAttach1"), CurrentWeapon->GetRootComponent(),
			FVector::ZeroVector, FVector(2.0f, 2.0f, 1.0f)) };
		Effect->AddManagedEffect(TEXT("DrawSword_SwordAttach1"), SwordAttach1);
		break;
	}
	}
}

void ATTPlayer::EndInit()
{
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PlayerDodgeMontage")):
	case FTTWorld::HashCode(TEXT("PlayerBackMoveMontage")):
	case FTTWorld::HashCode(TEXT("PlayerSlidingSlashAttackMontage")):
		StopGhostTrail(GetMesh());
		break;
	case FTTWorld::HashCode(TEXT("PlayerDrawSwordAttackMontage")):
		CurrentWeapon->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		break;
	}

}

void ATTPlayer::Attack()
{
	if (bIsAttacking)
	{
		TTCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (bCanNextCombo) bIsComboInputOn = true;
	}
	else if (GetCurrentStateNodeName() == TEXT("Ground")
		&& !TTAnimInstance->GetCurrentActiveMontage() && CurrentState == ECharacterState::BATTLE)
	{
		TTCHECK(!CurrentCombo);
		AttackStartComboState();
		TTAnimInstance->PlayMontage(TEXT("BasicAttack"));
		TTAnimInstance->JumpToAttackMontageSection(CurrentCombo);
		bIsAttacking = true;
	}
}

void ATTPlayer::SkillAttack(int32 SkillAttackType)
{
	if (!bIsDodging && !bIsSwappingWeapon && !bIsKnockBacking
		&& GetCurrentStateNodeName() == TEXT("Ground") && CurrentState == ECharacterState::BATTLE)
	{
		switch (SkillAttackType)
		{
		case 0:
			if (!bIsSkillAttacking[0])
			{
				TTAnimInstance->PlayMontage(TEXT("SmashAttack"));
				bIsSkillAttacking[0] = true;
			}
			break;
		case 1:
			if (!bIsSkillAttacking[1])
			{
				TTAnimInstance->PlayMontage(TEXT("SlidingSlashAttack"));
				bIsSkillAttacking[1] = true;
			}
			break;
		case 2:
			if (!bIsSkillAttacking[2])
			{
				TTAnimInstance->PlayMontage(TEXT("WindCutterAttack"));
				bIsSkillAttacking[2] = true;
			}
			break;
		case 3:
			if (!bIsSkillAttacking[3])
			{
				TTAnimInstance->PlayMontage(TEXT("GaiaCrushAttack"));
				bIsSkillAttacking[3] = true;
			}
			break;
		case 4:
			if (!bIsSkillAttacking[4])
			{
				TTAnimInstance->PlayMontage(TEXT("DrawSwordAttack"));
				bIsSkillAttacking[4] = true;
			}
			break;
		}
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
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PlayerAttackMontage")):
		if (CurrentCombo == 4)
		{
			AttackLength = 1.0f;
			AttackRadius = 330.0f;
		}
		else
		{
			AttackLength = 300.0f;
			AttackRadius = 100.0f;
			HitStartLocation = GetActorForwardVector() * AttackRadius;
		}
		break;
	case FTTWorld::HashCode(TEXT("PlayerSmashAttackMontage")):
		AttackLength = 300.0f;
		AttackRadius = 120.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("PlayerSlidingSlashAttackMontage")):
		AttackLength = 1200.0f;
		AttackRadius = 200.0f;
		HitStartLocation = -GetActorForwardVector() * (AttackLength - 300.0f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerWindCutterAttackMontage")):
		AttackLength = 100.0f;
		AttackRadius = 300.0f;
		HitStartLocation = GetActorForwardVector() * (AttackRadius - 100.0f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerGaiaCrushAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 600.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("PlayerDrawSwordAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 800.0f;
		break;
	}

	TArray<FOverlapResult> HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	FVector Trace{ GetActorForwardVector() * AttackLength };
	FVector Center{ GetActorLocation() + Trace * 0.5f + HitStartLocation };
	FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };

	bool bResult = GetWorld()->OverlapMultiByChannel(
		HitResult,
		Center,
		CapsuleRot,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeCapsule(AttackRadius, AttackLength),
		Params);

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PlayerAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FDamageEvent DamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 8.0f);
				}
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.5f);
			Audio->PlaySoundCue2D(TEXT("HitAttack"));
		}
		Audio->PlaySoundCue2D(TEXT("Attack"));
		break;
	case FTTWorld::HashCode(TEXT("PlayerSmashAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FDamageEvent DamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk() * 3.0f, DamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 8.0f);
				}
			Audio->PlaySoundWave2D(TEXT("HitSmashAttack"));
		}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 3.0f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerSlidingSlashAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FPointDamageEvent CriticalDamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk() * 1.5f, CriticalDamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 8.0f);
				}
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 3.0f);
			Audio->PlaySoundWave2D(TEXT("HitSlidingSlash"));
		}
		Audio->PlaySoundCue2D(TEXT("SlidingSlash"));
		break;
	case FTTWorld::HashCode(TEXT("PlayerWindCutterAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FPointDamageEvent CriticalDamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk() * 2.0f, CriticalDamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 8.0f);
				}
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 3.0f);
			Audio->PlaySoundWave2D(TEXT("HitWindCutter"));
		}
		break;
	case FTTWorld::HashCode(TEXT("PlayerGaiaCrushAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FPointDamageEvent CriticalDamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk() * 5.0f, CriticalDamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 15.0f);
				}
			Effect->PlayEffectAtLocation(TEXT("GaiaImpact"), GetActorLocation() + HitStartLocation, 5.0f);
			Audio->PlaySoundWave2D(TEXT("HitDrawSword"));
		}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		Effect->PlayEffectAtLocation(TEXT("GaiaLightning"), GetActorLocation() + HitStartLocation, 5.0f);
		break;
	case FTTWorld::HashCode(TEXT("PlayerDrawSwordAttackMontage")):
		if (bResult)
		{
			for (const auto& Result : HitResult)
				if (Result.Actor.IsValid())
				{
					FPointDamageEvent CriticalDamageEvent{};
					Result.Actor->TakeDamage(CharacterStat->GetAtk() * 10.0f, CriticalDamageEvent, GetController(), this);
					Effect->PlayEffectAtLocation(TEXT("HitImpact"), Result.GetActor()->GetActorLocation(), 15.0f);
				}
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 3.0f);
			Audio->PlaySoundWave2D(TEXT("HitDrawSword"));
			SetPlayRate(0.0f, 0.15f, 0.1f);
		}
		break;
	}

	if (FTTWorld::bIsDebugging)
	{
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FColor DrawColor{ bResult ? FColor::Green : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTPlayer::TurnToTarget(AActor* Target, float InterpSpeed)
{
	FVector DirectionToTarget{ Target->GetActorLocation() - GetActorLocation() };
	FRotator TargetRot{ FRotationMatrix::MakeFromX(DirectionToTarget.GetSafeNormal2D()).Rotator() };

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), InterpSpeed));
}

void ATTPlayer::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	StopGhostTrail(GetMesh());	// 임시코드

	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("PlayerAttackMontage")):
		bIsAttacking = false;
		AttackEndComboState();
		break;
	case FTTWorld::HashCode(TEXT("PlayerSmashAttackMontage")):
		bIsSkillAttacking[0] = false;
		break;
	case FTTWorld::HashCode(TEXT("PlayerSlidingSlashAttackMontage")):
		bIsSkillAttacking[1] = false;
		Effect->DeleteManagedEffect(TEXT("SlidingSlash_SwordAttach2"));
		break;
	case FTTWorld::HashCode(TEXT("PlayerWindCutterAttackMontage")):
		bIsSkillAttacking[2] = false;
		Effect->DeleteManagedEffect(TEXT("WindCutter_SwordAttach1"));
		Effect->DeleteManagedEffect(TEXT("WindCutter_SwordAttach2"));
		break;
	case FTTWorld::HashCode(TEXT("PlayerGaiaCrushAttackMontage")):
		bIsSkillAttacking[3] = false;
		break;
	case FTTWorld::HashCode(TEXT("PlayerDrawSwordAttackMontage")):
		bIsSkillAttacking[4] = false;
		Effect->DeleteManagedEffect(TEXT("DrawSword_SwordAttach1"));
		if (CurrentWeapon->GetActorRelativeScale3D().Size() != FMath::Sqrt(3))
			CurrentWeapon->SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		break;
	case FTTWorld::HashCode(TEXT("PlayerDodgeMontage")):
	case FTTWorld::HashCode(TEXT("PlayerBackMoveMontage")):
		bIsDodging = false;
		break;
	case FTTWorld::HashCode(TEXT("PlayerInWeaponMontage")):
	case FTTWorld::HashCode(TEXT("PlayerOutWeaponMontage")):
		bIsSwappingWeapon = false;
		break;
	case FTTWorld::HashCode(TEXT("PlayerKnockBackMontage")):
		bIsKnockBacking = false;
		break;
	}
}

ECharacterState ATTPlayer::GetCharacterState() const
{
	return CurrentState;
}

FName ATTPlayer::GetCurrentStateNodeName() const
{
	return TTAnimInstance->GetCurrentStateName(TTAnimInstance->GetStateMachineIndex(TEXT("BaseAction")));
}

void ATTPlayer::SetPlayRate(float StartTime, float EndTime, float TimeDilation)
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
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		DisableInput(TTPlayerController);
		TurnToTarget(LastDamageInstigator, 100.0f);
		TTAnimInstance->StopAllMontages(0.25f);
		TTAnimInstance->PlayMontage(TEXT("Death"));
		TTAnimInstance->SetDead();
		SetPlayRate(0.25f, 0.35f, 0.1f);

		FTimerHandle DeadTimerHandle{};
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
			[&]() { TTPlayerController->RestartLevel(); }), DeadTimer, false);
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

void ATTPlayer::Dodge(int32 DodgeType)
{
	if (!bIsDodging && !bIsSwappingWeapon && !bIsKnockBacking && GetCurrentStateNodeName() == TEXT("Ground"))
	{
		bIsDodging = true;

		switch (DodgeType)
		{
		case 1:
			TTAnimInstance->PlayMontage(TEXT("Dodge"));
			break;
		case 2:
			TTAnimInstance->PlayMontage(TEXT("BackMove"));
			break;
		default:
			bIsDodging = false;
			break;
		}
	}
}

void ATTPlayer::SwapBattleMode()
{
	if (GetCurrentStateNodeName() == TEXT("Ground") && !TTAnimInstance->GetCurrentActiveMontage())
	{
		if (TTAnimInstance->GetIsBattleOn())
		{
			TTAnimInstance->PlayMontage(TEXT("InWeapon"));
			SetCharacterState(ECharacterState::NOBATTLE);
			TTAnimInstance->SetIsBattleOn(false);
		}
		else
		{
			TTAnimInstance->PlayMontage(TEXT("OutWeapon"));
			SetCharacterState(ECharacterState::BATTLE);
			TTAnimInstance->SetIsBattleOn(true);
		}
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