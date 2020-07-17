#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIReinforce.generated.h"

UCLASS()
class THETREE_API UTTUIReinforce : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* AtkUpButton;
	UPROPERTY()
	class UButton* DefUpButton;
	UPROPERTY()
	class UButton* MaxHPUpButton;
	UPROPERTY()
	class UButton* MaxStaUpButton;
	UPROPERTY()
	class UButton* GetStaUpButton;

	UPROPERTY()
	class UTTCharacterStatComponent* CurrentCharacterStat;
	UPROPERTY()
	USoundWave* ReinforceSound;

	bool bIsClicking{};
protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);

	UFUNCTION()
	void OnAtkUp();
	UFUNCTION()
	void OnDefUp();
	UFUNCTION()
	void OnMaxHPUp();
	UFUNCTION()
	void OnMaxStaUp();
	UFUNCTION()
	void OnGetStaUp();
};
