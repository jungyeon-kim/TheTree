#include "TTArgoniteGiant.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTArgoniteGiant::ATTArgoniteGiant()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BasicEnemy/ArgoniteGiant/SK_Argonite_Giant.SK_Argonite_Giant") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGiant/ArgoniteGiantAnimBlueprint.ArgoniteGiantAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_ArcdevaWarrior_HitImpact.P_ArcdevaWarrior_HitImpact"));
	Audio->AddSoundCue(TEXT("Attack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_Attack_SoundCue.ArcdevaLancer_Attack_SoundCue"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitAttack_SoundCue.ArcdevaLancer_HitAttack_SoundCue"));
	Audio->AddSoundWave(TEXT("HitChargeAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitChargeAttack.ArcdevaLancer_HitChargeAttack"));
	Audio->AddSoundWave(TEXT("HitQuakeAttack"), TEXT("/Game/Assets/Sound/BasicEnemy/ArcdevaLancer/ArcdevaLancer_HitChargeAttack.ArcdevaLancer_HitChargeAttack"));

	GetCapsuleComponent()->SetCapsuleSize(80.0f, 140.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -140.0f));
	GeneralMoveSpeed = 450.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 10.0f;
}

void ATTArgoniteGiant::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("ArgoniteGiant"), GetGameInstance());

	TTAnimInstance->SetMontage(TEXT("HitReact"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGiant/ArgoniteGiantHitReactMontage.ArgoniteGiantHitReactMontage"));
	TTAnimInstance->SetMontage(TEXT("BasicAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGiant/ArgoniteGiantAttackMontage.ArgoniteGiantAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("ChargeAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGiant/ArgoniteGiantChargeAttackMontage.ArgoniteGiantChargeAttackMontage"));
	TTAnimInstance->SetMontage(TEXT("QuakeAttack"), TEXT("/Game/Blueprints/Animation/BasicEnemy/ArgoniteGiant/ArgoniteGiantQuakeAttackMontage.ArgoniteGiantQuakeAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTArgoniteGiant::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTArgoniteGiant::AttackCheck);
}

void ATTArgoniteGiant::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ArgoniteGiant.BT_ArgoniteGiant"));
}

void ATTArgoniteGiant::BeginPlay()
{
	Super::BeginPlay();
}

void ATTArgoniteGiant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTArgoniteGiant::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	if (!TTAnimInstance->GetCurrentActiveMontage() || DamageEvent.GetTypeID() == 1)
	{
		FVector LaunchVector{ GetActorLocation() - DamageCauser->GetActorLocation() };
		float ForceAmount{ 1300.0f };

		TurnToTarget(LastDamageInstigator, 100.0f);
		GetCharacterMovement()->Launch(LaunchVector.GetSafeNormal2D() * ForceAmount);

		TTAnimInstance->PlayMontage(TEXT("HitReact"));
	}

	return FinalDamage;
}

void ATTArgoniteGiant::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	FVector HitStartLocation{};
	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArgoniteGiantAttackMontage")):
		AttackLength = 100.0f;
		AttackRadius = 100.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	case FTTWorld::HashCode(TEXT("ArgoniteGiantChargeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 250.0f;
		break;
	case FTTWorld::HashCode(TEXT("ArgoniteGiantQuakeAttackMontage")):
		AttackLength = 1.0f;
		AttackRadius = 200.0f;
		HitStartLocation = GetActorForwardVector() * AttackRadius;
		break;
	}

	FHitResult HitResult{};
	FCollisionQueryParams Params{ NAME_None, false, this };
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + HitStartLocation,
		GetActorLocation() + GetActorForwardVector() * AttackLength + HitStartLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArgoniteGiantAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Audio->PlaySoundCueAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	case FTTWorld::HashCode(TEXT("ArgoniteGiantChargeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.0f, CriticalDamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 10.0f);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 10.0f);
				Audio->PlaySoundWave2D(TEXT("HitChargeAttack"));
			}
		break;
	}
	case FTTWorld::HashCode(TEXT("ArgoniteGiantQuakeAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FPointDamageEvent CriticalDamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk() * 2.5f, CriticalDamageEvent, GetController(), this);
				Effect->PlayEffectAtLocation(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(), 10.0f);
				Audio->PlaySoundWave2D(TEXT("HitQuakeAttack"));
			}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 5.0f);
		break;
	}
	}

	if (FTTWorld::bIsDebugging)
	{
		FVector Trace{ GetActorForwardVector() * AttackLength };
		FVector Center{ GetActorLocation() + Trace * 0.5f + HitStartLocation };
		float HalfHeight{ AttackLength * 0.5f + AttackRadius };
		FQuat CapsuleRot{ FRotationMatrix::MakeFromZ(Trace).ToQuat() };
		FColor DrawColor{ bResult ? FColor::Blue : FColor::Red };
		float DebugLifeTime{ 1.0f };
		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	}
}

void ATTArgoniteGiant::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ArgoniteGiantAttackMontage")):
	case FTTWorld::HashCode(TEXT("ArgoniteGiantChargeAttackMontage")):
	case FTTWorld::HashCode(TEXT("ArgoniteGiantQuakeAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}