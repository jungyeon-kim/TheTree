#include "TTAudioComponent.h"
#include "Sound/SoundCue.h"

UTTAudioComponent::UTTAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void UTTAudioComponent::AddSoundWave(FName SoundName, const TCHAR* SoundPath)
{
	const auto& SoundCheck{ LoadObject<USoundWave>(this, SoundPath) };

	if (SoundCheck->IsValidLowLevel()) SoundWave.Emplace(SoundName, SoundCheck);
	else TTLOG(Error, TEXT("Can't find SoundPath (%s)"), SoundPath);
}

void UTTAudioComponent::PlaySoundWave2D(FName SoundName)
{
	if (SoundWave.Find(SoundName)) UGameplayStatics::PlaySound2D(this, SoundWave[SoundName]);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName.ToString());
}

void UTTAudioComponent::PlaySoundWaveAtLocation(FName SoundName, FVector Location)
{
	if (SoundWave.Find(SoundName)) UGameplayStatics::PlaySoundAtLocation(this, SoundWave[SoundName], Location);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName.ToString());
}

void UTTAudioComponent::AddSoundCue(FName SoundName, const TCHAR* SoundPath)
{
	const auto& SoundCheck{ LoadObject<USoundCue>(this, SoundPath) };

	if (SoundCheck->IsValidLowLevel()) SoundCue.Emplace(SoundName, SoundCheck);
	else TTLOG(Error, TEXT("Can't find SoundPath (%s)"), SoundPath);
}

void UTTAudioComponent::PlaySoundCue2D(FName SoundName)
{
	if (SoundCue.Find(SoundName)) UGameplayStatics::PlaySound2D(this, SoundCue[SoundName]);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName.ToString());
}

void UTTAudioComponent::PlaySoundCueAtLocation(FName SoundName, FVector Location)
{
	if (SoundCue.Find(SoundName)) UGameplayStatics::PlaySoundAtLocation(this, SoundCue[SoundName], Location);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName.ToString());
}