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
	class USoundCue* SoundCue;
public:
	UTTAudioComponent();
	
	void PlaySound(const TCHAR* SoundPath);
};
