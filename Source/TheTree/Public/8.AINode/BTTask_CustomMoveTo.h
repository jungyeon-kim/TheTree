#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CustomMoveTo.generated.h"

UCLASS()
class THETREE_API UBTTask_CustomMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ATTEnemyBase* TTEnemy;
	UPROPERTY()
	class ATTPlayer* Target;
	UPROPERTY(EditAnywhere, Category = "Option")
	float AcceptableDistance;

	bool bIsMoving{};
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTTask_CustomMoveTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
