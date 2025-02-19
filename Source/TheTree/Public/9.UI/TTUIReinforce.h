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
	class UTextBlock* AtkGoldText;
	UPROPERTY()
	class UTextBlock* DefGoldText;
	UPROPERTY()
	class UTextBlock* MaxHPGoldText;
	UPROPERTY()
	class UTextBlock* MaxStaGoldText;
	UPROPERTY()
	class UTextBlock* GetStaGoldText;
	UPROPERTY()
	class UTextBlock* MyGoldText;

	UPROPERTY()
	TArray<int32> Price;
	int32 PriceIncrease{};

	UPROPERTY()
	class UTTCharacterStatComponent* CurrentCharacterStat;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	class UTTAudioComponent* Audio;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void Init();

	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);
	void UpdatePlayerGold();

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
