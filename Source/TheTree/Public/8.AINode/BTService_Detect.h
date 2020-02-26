#pragma once

#include "TheTree.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

UCLASS()
class THETREE_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Option")
	float DetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UBTService_Detect();
};
