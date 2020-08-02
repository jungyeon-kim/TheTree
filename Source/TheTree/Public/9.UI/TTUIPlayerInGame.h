#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIPlayerInGame.generated.h"

UCLASS()
class THETREE_API UTTUIPlayerInGame : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ATTPlayer* TTPlayer;
	UPROPERTY()
	class UTTCharacterStatComponent* CurrentCharacterStat;

	TArray<float> SkillCost;
	TArray<bool> bIsSkillEnabled;

	UPROPERTY()
	class UProgressBar* HPBar;
	UPROPERTY()
	class UProgressBar* StaBar;
	UPROPERTY()
	TArray<class UImage*> SkillIcon;
	UPROPERTY()
	class UTextBlock* PortalText;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);
	void ChangeSkillIconColor();
	void ShowPortalText();
};
