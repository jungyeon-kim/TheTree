#include "TTParticleSystemComponent.h"

UTTParticleSystemComponent::UTTParticleSystemComponent()
{
}

void UTTParticleSystemComponent::AddEffect(FString EffectName, const TCHAR* EffectPath)
{
	Effect.Emplace(EffectName, LoadObject<UParticleSystem>(this, EffectPath));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location);
	else TTLOG(Error, TEXT("Can't find EffectName"));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location, float Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, FRotator::ZeroRotator, FVector(Scale, Scale, Scale));
	else TTLOG(Error, TEXT("Can't find EffectName"));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location, FVector Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, FRotator::ZeroRotator, Scale);
	else TTLOG(Error, TEXT("Can't find EffectName"));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location, FRotator Rotation)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation);
	else TTLOG(Error, TEXT("Can't find EffectName"));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location, FRotator Rotation, float Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation, FVector(Scale, Scale, Scale));
	else TTLOG(Error, TEXT("Can't find EffectName"));
}

void UTTParticleSystemComponent::PlayEffect(FString EffectName, FVector Location, FRotator Rotation, FVector Scale)
{
	if (Effect.Find(EffectName)) UGameplayStatics::SpawnEmitterAtLocation(
		this, Effect[EffectName], Location, Rotation, Scale);
	else TTLOG(Error, TEXT("Can't find EffectName"));
}
