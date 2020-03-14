#include "TTImperfectDurion.h"
#include "TTEnemyAnimInstance.h"
#include "TTAIController.h"
#include "TTCameraShake.h"
#include "TTParticleSystemComponent.h"
#include "TTAudioComponent.h"
#include "TTCharacterStatComponent.h"
#include "DrawDebugHelpers.h"

ATTImperfectDurion::ATTImperfectDurion()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ENEMY{ TEXT("/Game/Assets/Character/BossEnemy/ImperfectDurion/SK_Imperfect_Durion.SK_Imperfect_Durion") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> ENEMY_ANIM{ TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionAnimBlueprint.ImperfectDurionAnimBlueprint_C") };
	if (SK_ENEMY.Succeeded()) GetMesh()->SetSkeletalMesh(SK_ENEMY.Object);
	if (ENEMY_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);

	Effect->AddEffect(TEXT("AttackImpact"), TEXT("/Game/ParagonMorigesh/FX/Particles/Morigesh/Abilities/LifeDrain/FX/P_ImperfectDurion_AttackImpact.P_ImperfectDurion_AttackImpact"));
	Effect->AddEffect(TEXT("HitImpact"), TEXT("/Game/Assets/Effect/Particle/P_PerfectDurion_HitImpact.P_PerfectDurion_HitImpact"));
	Effect->AddEffect(TEXT("Drain"), TEXT("/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_ImperfectDurion_Drain.P_ImperfectDurion_Drain"));
	Audio->AddSoundCue(TEXT("HitAttack"), TEXT("/Game/Assets/Sound/BossEnemy/PerfectDurion/Durion_HitAttack_SoundQue.Durion_HitAttack_SoundQue"));
	Audio->AddSoundWave(TEXT("Attack"), TEXT("/Game/Assets/Sound/BossEnemy/ImperfectDurion/ImperfectDurion_Attack.ImperfectDurion_Attack"));

	GetCapsuleComponent()->SetCapsuleSize(100.0f, 200.0f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));
	GeneralMoveSpeed = 400.0f;
	GetCharacterMovement()->MaxWalkSpeed = GeneralMoveSpeed;
	DeadTimer = 5.0f;
}

void ATTImperfectDurion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->SetObjectStat(TEXT("ImperfectDurion"));

	TTAnimInstance->SetMontage(EMontageType::ATTACK, TEXT("/Game/Blueprints/Animation/BossEnemy/ImperfectDurion/ImperfectDurionAttackMontage.ImperfectDurionAttackMontage"));
	TTAnimInstance->OnMontageEnded.AddDynamic(this, &ATTImperfectDurion::OnMontageEnded);
	TTAnimInstance->OnAttackHitCheck.AddUObject(this, &ATTImperfectDurion::AttackCheck);
	TTAnimInstance->OnPlayTalk.AddLambda([&]()
	{
		//Audio->PlaySoundCueAtLocation(TEXT("Talk"), GetActorLocation());
	});
}

void ATTImperfectDurion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TTAIController->SetBehaviorTree(TEXT("/Game/Blueprints/AI/BT_ImperfectDurion.BT_ImperfectDurion"));
}

void ATTImperfectDurion::BeginPlay()
{
	Super::BeginPlay();
}

void ATTImperfectDurion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATTImperfectDurion::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage{ Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) };
	TTLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage * (1.0f - CharacterStat->GetDef() / 100.0f));

	return FinalDamage;
}

void ATTImperfectDurion::AttackCheck()
{
	TTCHECK(TTAnimInstance->GetCurrentActiveMontage());

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
		AttackLength = 600.0f;
		AttackRadius = 100.0f;
		break;
	}

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

	switch (FTTWorld::HashCode(*GetCurrentMontage()->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
	{
		if (bResult)
			if (HitResult.Actor.IsValid())
			{
				FDamageEvent DamageEvent{};
				HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 2.0f);
				Effect->PlayEffect(TEXT("HitImpact"), HitResult.GetActor()->GetActorLocation(),
					GetActorForwardVector().Rotation(), 5.0f);
				Audio->PlaySoundCue2D(TEXT("HitAttack"));
			}
		Effect->PlayEffect(TEXT("AttackImpact"), GetActorLocation() + GetActorForwardVector() * AttackLength / 1.25f, 5.0f);
		Audio->PlaySoundWaveAtLocation(TEXT("Attack"), GetActorLocation());
		break;
	}
	}

	if (FTTWorld::bIsDebugging)
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

void ATTImperfectDurion::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	switch (FTTWorld::HashCode(*Montage->GetName()))
	{
	case FTTWorld::HashCode(TEXT("ImperfectDurionAttackMontage")):
		OnAttackEnded.Broadcast();
		break;
	}
}