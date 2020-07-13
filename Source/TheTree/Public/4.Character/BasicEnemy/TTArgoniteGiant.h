#pragma once

#include "TheTree.h"
#include "TTEnemyBase.h"
#include "TTArgoniteGiant.generated.h"

UCLASS()
class THETREE_API ATTArgoniteGiant : public ATTEnemyBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	void AttackCheck();

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	ATTArgoniteGiant();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
