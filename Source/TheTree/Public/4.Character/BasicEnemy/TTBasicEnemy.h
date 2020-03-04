#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTBasicEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class THETREE_API ATTBasicEnemy : public ACharacter
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
	class UTTBasicEnemyAnimInstance* TTAnimInstance;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackLength;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY()
	AActor* LastDamageInstigator;
	FTimerHandle DeadTimerHandle{};
protected:
	virtual void BeginPlay() override;

	void TurnToTarget(AActor* Target, float InterpSpeed);
public:
	UPROPERTY(VisibleAnywhere, Category = "Effect")
		class UTTParticleSystemComponent* Effect;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UTTCharacterStatComponent* CharacterStat;

	FOnAttackEndDelegate OnAttackEnded{};
public:
	ATTBasicEnemy();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void Attack();

	ECharacterState GetCharacterState() const;
	void SetCharacterState(ECharacterState NewState);
};
