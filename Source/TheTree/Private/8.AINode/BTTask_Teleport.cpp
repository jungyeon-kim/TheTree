#include "BTTask_Teleport.h"
#include "TTAIController.h"
#include "TTEnemyBase.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Teleport::UBTTask_Teleport()
{
	NodeName = TEXT("Teleport");
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result{ Super::ExecuteTask(OwnerComp, NodeMemory) };

	TTEnemy = Cast<ATTEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!TTEnemy) return EBTNodeResult::Failed;
	
	Target = Cast<ATTPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATTAIController::TargetKey));
	if (!Target) return EBTNodeResult::Failed;
	
	TTEnemy->PlayMontage(TeleportTypeName);
	TTEnemy->TeleportTo(Target->GetActorLocation() + FVector(0.0f, 0.0f, (TTEnemy->GetActorLocation().Z - Target->GetActorLocation().Z))
		+ Target->GetActorForwardVector(), (-Target->GetActorForwardVector()).Rotation());

	bIsTeleporting = true;
	TTEnemy->OnTeleportEnded.AddLambda([&]()
	{
		bIsTeleporting = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Teleport::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsTeleporting && !TTEnemy->GetCurrentMontage()) FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
