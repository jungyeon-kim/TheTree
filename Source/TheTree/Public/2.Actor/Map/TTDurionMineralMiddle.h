#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTDurionMineralMiddle.generated.h"

UCLASS()
class THETREE_API ATTDurionMineralMiddle : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTDurionMineralMiddle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MineralMesh;
};
