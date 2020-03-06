#include "BTTask_Defense.h"
#include "TTAIController.h"
#include "TTBasicEnemy.h"
#include "TTCharacterStatComponent.h"

UBTTask_Defense::UBTTask_Defense()
{
	NodeName = TEXT("Defense");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Defense::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTBasicEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;

	if (!TTEnemy->GetCurrentMontage())
	{
		float PrevDef{ TTEnemy->CharacterStat->GetDef() };

		TTEnemy->CharacterStat->SetDef(Def);
		TTEnemy->PlayMontage(DefenseTypeName);

		bIsDefensing = true;
		TTEnemy->OnDefenseEnded.AddLambda([&]()
		{
			TTEnemy->CharacterStat->SetDef(PrevDef);
			bIsDefensing = false;
		});
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Defense::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsDefensing && !TTEnemy->GetCurrentMontage()) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}