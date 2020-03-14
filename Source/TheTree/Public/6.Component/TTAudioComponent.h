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
	TMap<FName, USoundWave*> SoundWave;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TMap<FName, class USoundCue*> SoundCue;
public:
	UTTAudioComponent();
	
	void AddSoundWave(FName SoundName, const TCHAR* SoundPath);
	void PlaySoundWave2D(FName SoundName);
	void PlaySoundWaveAtLocation(FName SoundName, FVector Location);
	void AddSoundCue(FName SoundName, const TCHAR* SoundPath);
	void PlaySoundCue2D(FName SoundName);
	void PlaySoundCueAtLocation(FName SoundName, FVector Location);
};
