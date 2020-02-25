#pragma once

#include "TheTree.h"
#include "GameFramework/Character.h"
#include "TTBasicEnemy.generated.h"

UCLASS()
class THETREE_API ATTBasicEnemy : public ACharacter
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:	
	ATTBasicEnemy();

	virtual void Tick(float DeltaTime) override;
};
