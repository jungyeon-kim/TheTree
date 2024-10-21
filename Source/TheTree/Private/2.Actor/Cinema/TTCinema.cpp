#include "TTCinema.h"
#include "TTEnemyBase.h"
#include "TTAIController.h"
#include "TTPlayerController.h"
#include "TTUIPlayerInGame.h"
#include "LevelSequencePlayer.h"
#include "TTUIMap.h"
#include "TTPlayerWeapon.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ATTCinema::ATTCinema()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<ULevelSequence> SLS_FADE
	{ TEXT("/Game/Level/Cinema/CI_FadeIn.CI_FadeIn") };
	if (SLS_FADE.Succeeded())
		LevelSequence = SLS_FADE.Object;
}
void ATTCinema::SetCinema(class ULevelSequence* Sequence)
{
	if(Sequence)
	LevelSequence = Sequence;

	FMovieSceneSequencePlaybackSettings Settings{};
	ALevelSequenceActor* OutActor{};

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor);
	if (SequencePlayer)
	{
		FScriptDelegate EndFuncDelegate{};
		FScriptDelegate StartFuncDelegate{};

		StartFuncDelegate.BindUFunction(this, "StartCinemaFunction");
		EndFuncDelegate.BindUFunction(this, "EndCinemaFunction");
		SequencePlayer->OnPlay.Add(StartFuncDelegate);
		SequencePlayer->OnFinished.Add(EndFuncDelegate);
	}
}
void ATTCinema::SetCinema(const TCHAR* Path)
{
	LevelSequence = LoadObject<ULevelSequence>(ULevelSequence::StaticClass(), Path);
	FMovieSceneSequencePlaybackSettings Settings{};

	Settings.bDisableMovementInput = true;
	Settings.bDisableLookAtInput = true;
	Settings.bHideHud = true;

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

void ATTCinema::SetAndPlayCinema(ULevelSequence* Sequence, bool bRunAI, FName LevelName, bool bHidePlayer)
{
	bRunAIFlag = bRunAI;
	bHideCharacter = bHidePlayer;
	SetCinema(Sequence);
	OpenLevelName = LevelName;
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

	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->DisableInput(PlayerController);
	}

	if (PlayerController->GetUIPlayerInGame())
	{
		PlayerController->GetUIPlayerInGame()->SetVisibility(ESlateVisibility::Hidden);
	}

	TArray<AActor*> Arr{};

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTEnemyBase::StaticClass(), Arr);

	for (AActor* Enemy : Arr)
	{
		Cast<ATTAIController>(UAIBlueprintHelperLibrary::GetAIController(Enemy))->StopAI();
	}

	if (bHideCharacter)
	{
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorHiddenInGame(true);
		TArray<AActor*> Weapon{};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTPlayerWeapon::StaticClass(), Weapon);
		if (Weapon.Num())
			Weapon[0]->SetActorHiddenInGame(true);
	}
		
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

	if (bRunAIFlag)
		for (AActor*& Enemy : Arr)
			Cast<ATTEnemyBase>(Enemy)->GetController<ATTAIController>()->RunAI();

	if (bHideCharacter)
	{
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorHiddenInGame(false);
		TArray<AActor*> Weapon{};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATTPlayerWeapon::StaticClass(), Weapon);
		if (Weapon.Num())
			Weapon[0]->SetActorHiddenInGame(false);
	}
		

	if (OpenLevelName != "")
		UGameplayStatics::OpenLevel(GetWorld(), OpenLevelName);

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