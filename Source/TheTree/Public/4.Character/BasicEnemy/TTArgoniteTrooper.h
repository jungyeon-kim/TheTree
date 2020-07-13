#pragma once

#include "TheTree.h"
#include "TTEnemyBase.h"
#include "TTArgoniteTrooper.generated.h"

UCLASS()
class THETREE_API ATTArgoniteTrooper : public ATTEnemyBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void AttackCheck();

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	ATTArgoniteTrooper();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
