#include "TTPortal.h"
#include "TTPlayer.h"
#include "TTGameInstance.h"
#include "TTCinema.h"
#include "LevelSequencePlayer.h"

ATTPortal::ATTPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PORTAL"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_PORTALPARTICLE
	{ TEXT("/Game/Assets/Effect/Particle/P_Portal.P_Portal") };

	if (P_PORTALPARTICLE.Succeeded())
		PortalParticle->SetTemplate(P_PORTALPARTICLE.Object);

	RootComponent = PortalParticle;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OVERLAPBOX"));
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATTPortal::OnOverlapBegin);
	OverlapBox->SetSimulatePhysics(false);
	OverlapBox->BodyInstance.SetCollisionProfileName("Trigger");
	OverlapBox->SetRelativeScale3D(FVector{ 0.5f, 0.5f, 0.5f });
	OverlapBox->SetupAttachment(RootComponent);

	SetActorScale3D(FVector{ 4.0f, 4.0f, 4.0f });
}

void ATTPortal::BeginPlay()
{
	Super::BeginPlay();
}

void ATTPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTPortal::OnOverlapBegin(UPrimitiveComponent* OverlapComp , AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATTPlayer* Player{ Cast<ATTPlayer>(OtherActor) };
	if (Player)
	{
		if (*UGameplayStatics::GetCurrentLevelName(GetWorld()) == FString("ImperfectDurion_Battle"))
		{
			ATTCinema* FadeCinema{ GetWorld()->SpawnActor<ATTCinema>(ATTCinema::StaticClass()) };
			FadeCinema->SetCinema(TEXT("/Game/Level/Cinema/CI_FadeIn.CI_FadeIn"));
			FScriptDelegate Script{};
			Script.BindUFunction(this, "EndCinema");
			FadeCinema->GetSequencePlayer()->OnFinished.Add(Script);
			FadeCinema->PlayCinema();
		}
		else
			Player->SetUIMapOpenForced(true);
	}
}

void ATTPortal::EndCinema()
{
	UTTGameInstance* Inst{ GetGameInstance<UTTGameInstance>() };
	if (Inst)
	{
		switch(FTTWorld::HashCode(*UGameplayStatics::GetCurrentLevelName(GetWorld())))
		{
		case FTTWorld::HashCode(TEXT("ImperfectDurion_Battle")):
			UGameplayStatics::OpenLevel(GetWorld(), FName{ "PerfectDurion_Battle" });
			break;
		}
	}
}

