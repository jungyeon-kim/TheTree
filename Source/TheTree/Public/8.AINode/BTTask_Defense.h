#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Defense.generated.h"

UCLASS()
class THETREE_API UBTTask_Defense : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Option")
	EMontageType DefenseTypeName;
	UPROPERTY(EditAnywhere, Category = "Option")
	float Def;

	bool bIsDefensing{};
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTTask_Defense();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
