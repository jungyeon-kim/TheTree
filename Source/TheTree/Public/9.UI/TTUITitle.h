#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUITitle.generated.h"

UCLASS()
class THETREE_API UTTUITitle : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* StartButton;
	UPROPERTY()
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	USoundWave* ClickSound;
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStart();
	UFUNCTION()
	void OnExit();
};
