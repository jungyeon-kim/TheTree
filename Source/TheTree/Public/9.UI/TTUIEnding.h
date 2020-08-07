#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIEnding.generated.h"

UCLASS()
class THETREE_API UTTUIEnding : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
	USoundWave* ClickSound;

	bool bIsProcessing{};
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnExit();
};
