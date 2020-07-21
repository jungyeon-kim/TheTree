#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIPlayerStatus.generated.h"

UCLASS()
class THETREE_API UTTUIPlayerStatus : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UTextBlock* HPText;
	UPROPERTY()
	class UTextBlock* StaText;
	UPROPERTY()
	class UTextBlock* AtkText;
	UPROPERTY()
	class UTextBlock* DefText;
	UPROPERTY()
	class UTextBlock* GetStaText;
	UPROPERTY()
	class UTextBlock* MyGoldText;

	UPROPERTY()
	class UTTCharacterStatComponent* CurrentCharacterStat;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);
};
