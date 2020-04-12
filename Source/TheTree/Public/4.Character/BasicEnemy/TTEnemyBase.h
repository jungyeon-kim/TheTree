#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTEnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDefenseEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDodgeEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnTeleportEndDelegate);

UCLASS()
class THETREE_API ATTEnemyBase : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float GeneralMoveSpeed;
	UPROPERTY()
	class ATTAIController* TTAIController;
	UPROPERTY()
	class UTTEnemyAnimInstance* TTAnimInstance;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackLength;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY()
	AActor* LastDamageInstigator;
protected:
	virtual void BeginPlay() override;

	void TurnToTarget(AActor* Target, float InterpSpeed);
public:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<UCameraShake> CameraShake;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
		class UTTParticleSystemComponent* Effect;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UTTCharacterStatComponent* CharacterStat;

	FOnAttackEndDelegate OnAttackEnded{};
	FOnDefenseEndDelegate OnDefenseEnded{};
	FOnDodgeEndDelegate OnDodgeEnded{};
	FOnTeleportEndDelegate OnTeleportEnded{};
public:
	ATTEnemyBase();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void PlayMontage(FName MontageType);

	ECharacterState GetCharacterState() const;

	void SetPlayRate(float StartTime, float EndTime, float TimeDilation);
	void SetCharacterState(ECharacterState NewState);
};
