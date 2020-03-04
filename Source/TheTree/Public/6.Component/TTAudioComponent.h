#pragma once

#include "TheTree.h"
#include "Components/AudioComponent.h"
#include "TTAudioComponent.generated.h"

UCLASS()
class THETREE_API UTTAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TMap<FString, USoundWave*> SoundWave;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TMap<FString, class USoundCue*> SoundCue;
public:
	UTTAudioComponent();
	
	void AddSound(FString SoundName, const TCHAR* SoundPath);
	void PlaySound2D(FString SoundName);
	void PlaySoundAtLocation(FString SoundName, FVector Location);
	void AddSoundCue(FString SoundName, const TCHAR* SoundPath);
	void PlaySoundCue2D(FString SoundName);
	void PlaySoundCueAtLocation(FString SoundName, FVector Location);
};
