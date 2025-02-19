#include "TTUITitle.h"
#include "Components/Button.h"

void UTTUITitle::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("btnStart")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("btnExit")));

	StartButton->OnClicked.AddDynamic(this, &UTTUITitle::OnStart);
	ExitButton->OnClicked.AddDynamic(this, &UTTUITitle::OnExit);

	ClickSound = LoadObject<USoundWave>(this, TEXT("/Game/Assets/Sound/Common/Click_00.Click_00"));
}

void UTTUITitle::OnStart()
{
	UGameplayStatics::PlaySound2D(this, ClickSound);

	if (bIsProcessing) return;

	FTimerHandle TimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			UGameplayStatics::OpenLevel(this, TEXT("Lobby"));
		}), 0.3, false);

	bIsProcessing = true;
}

void UTTUITitle::OnExit()
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
