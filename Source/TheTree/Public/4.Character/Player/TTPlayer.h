#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTPlayer.generated.h"

UCLASS()
class THETREE_API ATTPlayer : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float GeneralMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float AdvancedMoveSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TArray<bool> bIsSkillAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	TArray<float> SkillCost;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackLength;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	bool bIsDodging;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	bool bIsSwappingWeapon;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "KnockBack", Meta = (AllowPrivateAccess = true))
	bool bIsKnockBacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool bIsOpenedMap;

	UPROPERTY()
	class ATTPlayerController* TTPlayerController;
	UPROPERTY()
	class UTTPlayerAnimInstance* TTAnimInstance;
	UPROPERTY()
	class UTTGhostTrailComponent* TTGhostTrail;

	FTimerHandle AttackCancelTimer = FTimerHandle();
private:
	void StartInit();
	void EndInit();
	void Attack(const bool bInAttacking);
	void SkillAttack(int32 SkillAttackType);
	void Dodge(int32 DodgeType);
	void SwapBattleMode();
	void Jump();
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void OpenUIPlayerStatus();
	void SetUIMapOpenImpl(bool bOpenMap);
	void OpenUIQuitGame();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void TurnToTarget(AActor* Target, float InterpSpeed);
protected:
	enum class EControlMode
	{
		THIRD_PERSON
	};
protected:
	UPROPERTY()
	AActor* LastDamageInstigator;
	EControlMode CurrentControlMode{};
protected:
	virtual void BeginPlay() override;
	void SetControlMode(EControlMode NewControlMode);
public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<UCameraShake> CameraShake;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	class UTTParticleSystemComponent* Effect;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class ATTPlayerWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UTTCharacterStatComponent* CharacterStat;
public:	
	ATTPlayer();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	void ResetPlayer();

	ECharacterState GetCharacterState() const;
	FName GetCurrentStateNodeName() const;
	TArray<float> GetSkillCost() const;

	void SetPlayRate(float StartTime, float EndTime, float TimeDilation);
	void SetWeapon();
	void SetCharacterState(ECharacterState NewState);
	void SetUIMapOpenFlipFlop();
	void SetUIMapOpenForced(bool bOpenMap);
};