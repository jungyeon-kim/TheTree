#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Dodge.generated.h"

UCLASS()
class THETREE_API UBTTask_Dodge : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ATTEnemyBase* TTEnemy;
	UPROPERTY(EditAnywhere, Category = "Option")
	FName DodgeTypeName;

	bool bIsDodging{};
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTTask_Dodge();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};