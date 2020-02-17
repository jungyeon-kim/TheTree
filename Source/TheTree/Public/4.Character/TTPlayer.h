#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class THETREE_API ATTPlayer : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	UPROPERTY()
	class ATTPlayerController* TTPlayerController;
	UPROPERTY()
	class UTTAnimInstance* TTAnimInstance;

	FTimerHandle DeadTimerHandle{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;		// 공격중인지 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bCanNextCombo;		// 다음 콤보 실행가능 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsComboInputOn;	// 콤보 입력 여부
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;		// 현재 실행중인 콤보
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;			// 콤보의 최대치
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackLength;		// 공격판정 길이
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;		// 공격판정 반지름
private:
	void Jump();
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
protected:
	enum class EControlMode
	{
		THIRD_PERSON
	};
protected:
	float ArmLengthTo{};
	float ArmLengthSpeed{};
	float ArmRotationSpeed{};
	FRotator ArmRotationTo{};
	EControlMode CurrentControlMode{};
	FVector DirectionToMove{};
protected:
	virtual void BeginPlay() override;
	void SetControlMode(EControlMode NewControlMode);
public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;
	//UPROPERTY(VisibleAnywhere, Category = "UI")
	//class UWidgetComponent* HPBarWidget;
	//UPROPERTY(VisibleAnywhere, Category = "Stat")
	//class UTTCharacterStatComponent* CharacterStat;
	//UPROPERTY(VisibleAnywhere, Category = "Weapon")
	//class ATTWeapon* CurrentWeapon;
public:	
	ATTPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	void Attack();
	FOnAttackEndDelegate OnAttackEnded{};

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);

	ECharacterState GetCharacterState() const;
	void SetCharacterState(ECharacterState NewState);
	FName GetCurrentStateMachineName() const;
};
