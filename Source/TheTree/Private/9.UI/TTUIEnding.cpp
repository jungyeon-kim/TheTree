#include "TTUIEnding.h"
#include "Components/Button.h"

void UTTUIEnding::NativeConstruct()
{
	Super::NativeConstruct();

	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExit")));

	ExitButton->OnClicked.AddDynamic(this, &UTTUIEnding::OnExit);

	ClickSound = LoadObject<USoundWave>(this, TEXT("/Game/Assets/Sound/Common/Click_00.Click_00"));
}

void UTTUIEnding::OnExit()
{
	UGameplayStatics::PlaySound2D(this, ClickSound);

	if (bIsProcessing) return;

	FTimerHandle TimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
		}), 0.3, false);

	bIsProcessing = true;
}
