#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

UCLASS()
class THETREE_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
