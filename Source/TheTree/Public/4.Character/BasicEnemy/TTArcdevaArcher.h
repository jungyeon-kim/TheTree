#pragma once

#include "TheTree.h"
#include "TTEnemyBase.h"
#include "TTArcdevaArcher.generated.h"

UCLASS()
class THETREE_API ATTArcdevaArcher : public ATTEnemyBase
{
	GENERATED_BODY()
private:
	FVector AttackStartForwardVector{};
protected:
	virtual void BeginPlay() override;
	void AttackStart();
	void AttackCheck();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	ATTArcdevaArcher();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
