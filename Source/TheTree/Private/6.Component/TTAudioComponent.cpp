#include "TTAudioComponent.h"
#include "Sound/SoundCue.h"

UTTAudioComponent::UTTAudioComponent()
{
	GamePlay = CreateDefaultSubobject<UGameplayStatics>("GAMEPLAY");
}

void UTTAudioComponent::PlaySound(const TCHAR* SoundPath)
{
	SoundCue = LoadObject<USoundCue>(NULL, SoundPath);
	GamePlay->PlaySound2D(this, SoundCue);
}