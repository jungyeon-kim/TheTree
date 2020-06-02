#include "TTCinema.h"
#include "TTEnemyBase.h"
#include "TTAIController.h"
#include "TTPlayerController.h"
#include "TTUIPlayerInGame.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ATTCinema::ATTCinema()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<ULevelSequence> SLS_FADE
	{ TEXT("/Game/Level/Cinema/CI_FadeOut.CI_FadeOut") };
	if (SLS_FADE.Succeeded())
		LevelSequence = SLS_FADE.Object;
}
void ATTCinema::SetCinema(class ULevelSequence* Sequence)
{
	if(Sequence)
	LevelSequence = Sequence;
	FMovieSceneSequencePlaybackSettings Settings{};

	Settings.bDisableMovementInput = true;
	Settings.bDisableLookAtInput = true;

	ALevelSequenceActor* OutActor{ nullptr };

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor);
	if (SequencePlayer)
	{
		FScriptDelegate EndFuncDelegate{};
		FScriptDelegate StartFuncDelegate{};

		StartFuncDelegate.BindUFunction(this, "StartCinemaFunction");
		EndFuncDelegate.BindUFunction(this, "EndCinemaFunction");
		SequencePlayer->OnPlay.AddUnique(StartFuncDelegate);
		SequencePlayer->OnFinished.AddUnique(EndFuncDelegate);
	}
}
void ATTCinema::SetCinema(const TCHAR* Path)
{
	LevelSequence = LoadObject<ULevelSequence>(ULevelSequence::StaticClass(), Path);
	FMovieSceneSequencePlaybackSettings Settings{};

	Settings.bDisableMovementInput = true;
	Settings.bDisableLookAtInput = true;

	ALevelSequenceActor* OutActor{ nullptr };

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor);
	if (SequencePlayer)
	{
		FScriptDelegate EndFuncDelegate{};
		FScriptDelegate StartFuncDelegate{};

		StartFuncDelegate.BindUFunction(this, "StartCinemaFunction");
		EndFuncDelegate.BindUFunction(this, "EndCinemaFunction");
		SequencePlayer->OnPlay.AddUnique(StartFuncDelegate);
		SequencePlayer->OnFinished.AddUnique(EndFuncDelegate);
	}
}

void ATTCinema::PlayCinema()
{
	SequencePlayer->Play();
}

void ATTCinema::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATTCinema::BeginDestroy()
{
	Super::BeginDestroy();
}

void ATTCinema::SetAndPlayCinema(const TCHAR* Path)
{
	SetCinema(Path);
	PlayCinema();
}

void ATTCinema::SetAndPlayCinema(ULevelSequence* Sequence)
{
	SetCinema(Sequence);
	PlayCinema();
}

ULevelSequencePlayer* ATTCinema::GetSequencePlayer()
{
	return SequencePlayer;
}

void ATTCinema::StartCinemaFunction()
{
	static ATTPlayerController* PlayerController{};
	PlayerController = Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController());

	TTCHECK(PlayerController);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(PlayerController);
	if (!PlayerController->GetUIPlayerInGame()) PlayerController->OnSyncDelegate.AddLambda([&]() {
		PlayerController->GetUIPlayerInGame()->SetVisibility(ESlateVisibility::Hidden); });
	else PlayerController->GetUIPlayerInGame()->SetVisibility(ESlateVisibility::Hidden);

	TArray<AActor*> Arr{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		ATTEnemyBase::StaticClass(), Arr);

	for (AActor* Enemy : Arr)
		Cast<ATTAIController>(UAIBlueprintHelperLibrary::GetAIController(Enemy))
		->StopAI();
}

void ATTCinema::EndCinemaFunction()
{
	ATTPlayerController* PlayerController{ Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController()) };

	TTCHECK(PlayerController);
	PlayerController->GetUIPlayerInGame()->SetVisibility(ESlateVisibility::Visible);
	
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->
		EnableInput(GetWorld()->GetFirstPlayerController());

	TArray<AActor*> Arr{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		ATTEnemyBase::StaticClass(), Arr);

	for (AActor*& Enemy : Arr)
		Cast<ATTEnemyBase>(Enemy)->GetController<ATTAIController>()->RunAI();

	
	Destroy();
}

void ATTCinema::BeginPlay()
{
	Super::BeginPlay();
}

void ATTCinema::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}