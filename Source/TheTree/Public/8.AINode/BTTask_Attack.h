#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

UCLASS()
class THETREE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Option")
	EMontageType AttackTypeName;

	bool bIsAttacking{};
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
