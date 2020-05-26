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
	class UTTCharacterStatComponent* CurrentCharacterStat{};
	UPROPERTY()
	class UProgressBar* HPBar;
	UPROPERTY()
	class UProgressBar* StaBar;
protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
public:
	void BindCharacterStat(class UTTCharacterStatComponent* CharacterStat);
};
