#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTDurionMineralLarge.generated.h"

UCLASS()
class THETREE_API ATTDurionMineralLarge : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTDurionMineralLarge();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MineralMesh;
};
