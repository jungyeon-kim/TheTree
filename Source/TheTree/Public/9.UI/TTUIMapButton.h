#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Components/Button.h"
#include "TTUIMapButton.generated.h"

UCLASS(Blueprintable)
class THETREE_API UTTUIMapButton : public UButton
{
	GENERATED_BODY()

	UTTUIMapButton();

private:
	FVector2D OriginPosition;

public:
	void SetButtonType(USlateWidgetStyleAsset* MapWidgetStyle);
	void SetOriginPosition(const FVector2D& Coordinate);
	inline FVector2D GetOriginPosition()
	{
		return OriginPosition;
	}
};
