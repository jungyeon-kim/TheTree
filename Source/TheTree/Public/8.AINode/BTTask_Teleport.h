#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Teleport.generated.h"

UCLASS()
class THETREE_API UBTTask_Teleport : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ATTEnemyBase* TTEnemy;
	UPROPERTY()
	class ATTPlayer* Target;
	UPROPERTY(EditAnywhere, Category = "Option")
	FName TeleportTypeName;

	bool bIsTeleporting{};
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTTask_Teleport();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
