#include "BTService_Detect.h"
#include "TTAIController.h"
#include "TTPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto& ControllingPawn{ OwnerComp.GetAIOwner()->GetPawn() };
	if (!ControllingPawn) return;

	const auto& World{ ControllingPawn->GetWorld() };
	FVector Center{ ControllingPawn->GetActorLocation() };
	float DetectRadius{ 1200.0f };
	if (!World) return;

	TArray<FOverlapResult> OverlapResults{};
	FCollisionQueryParams CollisionQueryParam{ NAME_None, false, ControllingPawn };

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
		for (const auto& OverlapResult : OverlapResults)
		{
			const auto& TTPlayer{ Cast<ATTPlayer>(OverlapResult.GetActor()) };

			if (TTPlayer)
			{
				if (TTPlayer->GetController()->IsPlayerController())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATTAIController::TargetKey, TTPlayer);
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Blue, false, 0.2f);
					return;
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATTAIController::TargetKey, nullptr);
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
				}
			}
		}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATTAIController::TargetKey, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}

}