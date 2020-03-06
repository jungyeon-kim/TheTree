#include "BTTask_Attack.h"
#include "TTAIController.h"
#include "TTBasicEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTBasicEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;
	
	if (!TTEnemy->GetCurrentMontage())
	{
		TTEnemy->PlayMontage(AttackTypeName);

		bIsAttacking = true;
		TTEnemy->OnAttackEnded.AddLambda([&]()
		{
			bIsAttacking = false;
		});
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsAttacking && !TTEnemy->GetCurrentMontage()) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
