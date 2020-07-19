#pragma once

#include "Runtime/UMG/Public/UMG.h"
#include "Components/Button.h"
#include "TTUIMapButton.generated.h"


UCLASS(Blueprintable)
class THETREE_API UTTUIMapButton : public UButton
{
	GENERATED_BODY()

	UTTUIMapButton();

	void SetButtonType();
};
