#pragma once

#include "TheTree.h"
#include "AIController.h"
#include "TTAIController.generated.h"

enum class EAIType
{
	BASIC
};

UCLASS()
class THETREE_API ATTAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UBlackboardData* BBAsset;
	UPROPERTY()
	class UBehaviorTree* BTAsset;
public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
public:
	ATTAIController();
	virtual void OnPossess(APawn* InPawn) override;

	void RunAI();
	void StopAI();

	void SetBehaviorTree(EAIType AIType, const TCHAR* BehaviroTreePath);
};
