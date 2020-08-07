#pragma once

#include "TheTree.h"
#include "TTEnemyBase.h"
#include "TTTrooper.generated.h"

UCLASS()
class THETREE_API ATTTrooper : public ATTEnemyBase
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class ATTPlayerController* TTPlayerController;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AttackCheck();

	UFUNCTION(BlueprintCallable)
	void ShowBossHPBar();
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	ATTTrooper();

	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};
