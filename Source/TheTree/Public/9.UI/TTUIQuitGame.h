#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIQuitGame.generated.h"

UCLASS()
class THETREE_API UTTUIQuitGame : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* YesButton;
	UPROPERTY()
	class UButton* NoButton;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	USoundWave* ClickSound;

	bool* bIsOpened{};
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnYes();
	UFUNCTION()
	void OnNo();
public:
	void BindIsOpened(bool& NewIsOpened);
};
