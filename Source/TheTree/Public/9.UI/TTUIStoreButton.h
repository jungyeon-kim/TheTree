#pragma once

#include "9.UI/TTUIMapButton.h"
#include "TTUIStoreButton.generated.h"

UCLASS()
class THETREE_API UTTUIStoreButton : public UTTUIMapButton
{
	GENERATED_BODY()

	UTTUIStoreButton();
public:
	virtual void OnClickEvent() override;
	
};
