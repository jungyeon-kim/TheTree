#pragma once

#include "TheTree.h"
#include "GameFramework/Actor.h"
#include "TTTorch.generated.h"

UCLASS()
class THETREE_API ATTTorch : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTTorch();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* TorchMesh;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UPointLightComponent* TorchLight;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* TorchParticle;
};
