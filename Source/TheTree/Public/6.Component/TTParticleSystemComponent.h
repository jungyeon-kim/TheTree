#pragma once

#include "TheTree.h"
#include "Particles/ParticleSystemComponent.h"
#include "TTParticleSystemComponent.generated.h"

UCLASS()
class THETREE_API UTTParticleSystemComponent : public UParticleSystemComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TMap<FString, UParticleSystem*> Effect;
public:
	UTTParticleSystemComponent();

	void AddEffect(FString EffectName, const TCHAR* EffectPath);
	void PlayEffect(FString EffectName, FVector Location);
	void PlayEffect(FString EffectName, FVector Location, float Scale);
	void PlayEffect(FString EffectName, FVector Location, FVector Scale);
	void PlayEffect(FString EffectName, FVector Location, FRotator Rotation);
	void PlayEffect(FString EffectName, FVector Location, FRotator Rotation, float Scale);
	void PlayEffect(FString EffectName, FVector Location, FRotator Rotation, FVector Scale);
};
