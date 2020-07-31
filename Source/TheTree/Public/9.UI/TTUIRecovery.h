#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIRecovery.generated.h"

UCLASS()
class THETREE_API UTTUIRecovery : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* RecoveryHPButton;
	UPROPERTY()
	class UButton* RecoveryStaButton;

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

	UFUNCTION()
	void OnRecoveryHP();
	UFUNCTION()
	void OnRecoverySta();
};
