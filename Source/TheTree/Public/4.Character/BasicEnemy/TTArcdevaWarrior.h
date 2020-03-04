#pragma once

#include "TheTree.h"
#include "TTBasicEnemy.h"
#include "TTArcdevaWarrior.generated.h"

UCLASS()
class THETREE_API ATTArcdevaWarrior : public ATTBasicEnemy
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void AttackCheck();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	ATTArcdevaWarrior();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
