#include "TTUIQuitGame.h"
#include "Components/Button.h"
#include "TTPlayer.h"

void UTTUIQuitGame::NativeConstruct()
{
	Super::NativeConstruct();

	YesButton = Cast<UButton>(GetWidgetFromName(TEXT("btnYes")));
	NoButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNo")));

	YesButton->OnClicked.AddDynamic(this, &UTTUIQuitGame::OnYes);
	NoButton->OnClicked.AddDynamic(this, &UTTUIQuitGame::OnNo);

	ClickSound = LoadObject<USoundWave>(this, TEXT("/Game/Assets/Sound/Common/Click_00.Click_00"));
}

void UTTUIQuitGame::BindIsOpened(bool& NewIsOpened)
{
	bIsOpened = &NewIsOpened;
}

void UTTUIQuitGame::OnYes()
{
	UGameplayStatics::PlaySound2D(this, ClickSound);

	if (!(*bIsOpened)) return;

	FTimerHandle TimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Cast<ATTPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0))->ResetPlayer();
			UGameplayStatics::OpenLevel(this, TEXT("Title"));
		}), 0.3, false);

	*bIsOpened = false;
}

void UTTUIQuitGame::OnNo()
{
	UGameplayStatics::PlaySound2D(this, ClickSound);

	if (!(*bIsOpened)) return;

	const auto PlayerController{ UGameplayStatics::GetPlayerController(this, 0) };
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetInputMode(FInputModeGameOnly{});
	SetVisibility(ESlateVisibility::Hidden);

	*bIsOpened = false;
}
