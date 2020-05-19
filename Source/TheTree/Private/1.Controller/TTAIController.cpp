#include "TTAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATTAIController::HomePosKey{ TEXT("HomePos") };
const FName ATTAIController::PatrolPosKey{ TEXT("PatrolPos") };
const FName ATTAIController::TargetKey{ TEXT("Target") };

ATTAIController::ATTAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_Object{ TEXT("/Game/Blueprints/AI/BB_Enemy.BB_Enemy") };
	if (BB_Object.Succeeded()) BBAsset = BB_Object.Object;
}

void ATTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ATTAIController::RunAI()
{
	TTCHECK(BTAsset && BBAsset);
	if (UseBlackboard(BBAsset, Blackboard)) Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
	RunBehaviorTree(BTAsset);
}

void ATTAIController::StopAI()
{
	const auto& BehaviorTreeComponent{ Cast<UBehaviorTreeComponent>(BrainComponent) };
	
	if (BehaviorTreeComponent)
		BehaviorTreeComponent->StopTree();
}

void ATTAIController::SetBehaviorTree(const TCHAR* BehaviroTreePath)
{
	const auto& TreeCheck{ LoadObject<UBehaviorTree>(this, BehaviroTreePath) };

	if (TreeCheck->IsValidLowLevel()) BTAsset = TreeCheck;
	else TTLOG(Error, TEXT("Can't find BehaviroTreePath (%s)"), BehaviroTreePath);
}
