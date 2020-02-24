#include "TTAudioComponent.h"
#include "Sound/SoundCue.h"

UTTAudioComponent::UTTAudioComponent()
{
	GamePlay = CreateDefaultSubobject<UGameplayStatics>("GAMEPLAY");
}

void UTTAudioComponent::AddSound(FString SoundName, const TCHAR* SoundPath)
{
	SoundCue.Emplace(SoundName, LoadObject<USoundCue>(NULL, SoundPath));
}

void UTTAudioComponent::PlaySound2D(FString SoundName)
{
	if (SoundCue.Find(SoundName)) GamePlay->PlaySound2D(this, SoundCue[SoundName]);
	else TTLOG(Error, TEXT("Can't find SoundName"));
}

void UTTAudioComponent::PlaySoundAtLocation(FString SoundName, FVector Location)
{
	if (SoundCue.Find(SoundName)) GamePlay->PlaySoundAtLocation(this, SoundCue[SoundName], Location);
	else TTLOG(Error, TEXT("Can't find SoundName"));
}