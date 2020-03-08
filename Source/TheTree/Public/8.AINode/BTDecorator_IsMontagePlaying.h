#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsMontagePlaying.generated.h"

UCLASS()
class THETREE_API UBTDecorator_IsMontagePlaying : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsMontagePlaying();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
