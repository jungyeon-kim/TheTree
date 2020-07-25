#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Components/Button.h"
#include "TTUIMapButton.generated.h"

UCLASS(Blueprintable)
class THETREE_API UTTUIMapButton : public UButton
{
	GENERATED_BODY()

public:
	UTTUIMapButton();

protected:
	FVector2D OriginPosition;
	TArray<UButton*> Childs;
	UTTUIMapButton* Parent;
	UPROPERTY()
	UWorld* WorldContext;
public:
	void SetButtonType(USlateWidgetStyleAsset* MapWidgetStyle);
	void SetOriginPosition(const FVector2D& Coordinate);
	void RegistChild(UTTUIMapButton* Child);
	void SetParent(UTTUIMapButton* Button);
	void SetWorldContext(UWorld* World);
	void SetEnableChilds(bool bFlag);
	inline FVector2D GetOriginPosition() const
	{
		return OriginPosition;
	}

	UFUNCTION()
	virtual void OnClickEvent();
};
