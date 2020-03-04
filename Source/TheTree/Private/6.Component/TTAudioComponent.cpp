#include "TTAudioComponent.h"
#include "Sound/SoundCue.h"

UTTAudioComponent::UTTAudioComponent()
{
}

void UTTAudioComponent::AddSound(FString SoundName, const TCHAR* SoundPath)
{
	SoundWave.Emplace(SoundName, LoadObject<USoundWave>(this, SoundPath));
}

void UTTAudioComponent::PlaySound2D(FString SoundName)
{
	if (SoundWave.Find(SoundName)) UGameplayStatics::PlaySound2D(this, SoundWave[SoundName]);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName);
}

void UTTAudioComponent::PlaySoundAtLocation(FString SoundName, FVector Location)
{
	if (SoundWave.Find(SoundName)) UGameplayStatics::PlaySoundAtLocation(this, SoundWave[SoundName], Location);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName);
}

void UTTAudioComponent::AddSoundCue(FString SoundName, const TCHAR* SoundPath)
{
	SoundCue.Emplace(SoundName, LoadObject<USoundCue>(this, SoundPath));
}

void UTTAudioComponent::PlaySoundCue2D(FString SoundName)
{
	if (SoundCue.Find(SoundName)) UGameplayStatics::PlaySound2D(this, SoundCue[SoundName]);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName);
}

void UTTAudioComponent::PlaySoundCueAtLocation(FString SoundName, FVector Location)
{
	if (SoundCue.Find(SoundName)) UGameplayStatics::PlaySoundAtLocation(this, SoundCue[SoundName], Location);
	else TTLOG(Error, TEXT("Can't find SoundName (%s)"), *SoundName);
}