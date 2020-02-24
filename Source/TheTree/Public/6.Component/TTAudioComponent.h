#pragma once

#include "TheTree.h"
#include "Components/AudioComponent.h"
#include "TTAudioComponent.generated.h"

UCLASS()
class THETREE_API UTTAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Category = "World")
	UGameplayStatics* GamePlay;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TMap<FString, class USoundCue*> SoundCue;
public:
	UTTAudioComponent();
	
	void AddSound(FString SoundName, const TCHAR* SoundPath);
	void PlaySound2D(FString SoundName);
	void PlaySoundAtLocation(FString SoundName, FVector Location);
};
