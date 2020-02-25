#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

UCLASS()
class THETREE_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
