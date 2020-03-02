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

	FTimerHandle DeadTimerHandle{};
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;

	FOnAttackEndDelegate OnAttackEnded{};
public:
	ATTBasicEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	void Attack();

	ECharacterState GetCharacterState() const;
	void SetCharacterState(ECharacterState NewState);
};
