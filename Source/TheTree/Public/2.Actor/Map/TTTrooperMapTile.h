#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTTrooperMapTile.generated.h"

UCLASS()
class THETREE_API ATTTrooperMapTile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTTrooperMapTile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MapMesh;
};
