#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTChandelier.generated.h"

UCLASS()
class THETREE_API ATTChandelier : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTChandelier();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ChandelierMesh;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UPointLightComponent* ChandelierLight;
};
