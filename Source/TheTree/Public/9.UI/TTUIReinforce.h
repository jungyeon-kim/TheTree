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

	bool bIsClicking{};
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);

	UFUNCTION()
	void OnStkUp();
};
