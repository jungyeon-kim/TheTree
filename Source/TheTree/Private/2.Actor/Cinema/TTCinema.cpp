#include "TTCinema.h"
#include "TTEnemyBase.h"
#include "TTAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ATTCinema::ATTCinema()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<ULevelSequence> SLS_FADE
	{ TEXT("/Game/Level/Cinema/Ci_FadeInOut.CI_FadeInOut") };
	if (SLS_FADE.Succeeded())
		LevelSequence = SLS_FADE.Object;
}

void ATTCinema::SetCinema(const TCHAR* Path)
{
	LevelSequence = LoadObject<ULevelSequence>(ULevelSequence::StaticClass(), Path);
	FMovieSceneSequencePlaybackSettings Settings{};

	Settings.bDisableMovementInput = true;
	Settings.bDisableLookAtInput = true;

	ALevelSequenceActor* outActor{ nullptr };

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, outActor);
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

void ATTCinema::SetAndPlayCinema(const TCHAR* Path)
{
	SetCinema(Path);
	PlayCinema();
}

void ATTCinema::StartCinemaFunction()
{
	UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->
		DisableInput(GetWorld()->GetFirstPlayerController());

	TArray<AActor*> Arr{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),
		ATTEnemyBase::StaticClass(), Arr);

	for (AActor* Enemy : Arr)
		Cast<ATTAIController>(UAIBlueprintHelperLibrary::GetAIController(Enemy))
		->StopAI();
}

void ATTCinema::EndCinemaFunction()
{
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
	SetAndPlayCinema(TEXT("/Game/Level/Cinema/Ci_FadeInOut.CI_FadeInOut"));
}

void ATTCinema::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



