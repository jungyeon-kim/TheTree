#include "TTHPBead.h"
#include "TTPlayer.h"
#include "TTCharacterStatComponent.h"
#include "TTAudioComponent.h"

ATTHPBead::ATTHPBead()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPHERE"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOUNDBOX"));
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));

	RootComponent = Sphere;
	Light->SetupAttachment(RootComponent);
	BoundBox->SetupAttachment(RootComponent);
	Audio->SetupAttachment(RootComponent);

	Sphere->SetCollisionProfileName(TEXT("NoCollision"));
	BoundBox->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SPHERE{ TEXT("/Engine/BasicShapes/Sphere.Sphere") };
	if (SM_SPHERE.Succeeded()) Sphere->SetStaticMesh(SM_SPHERE.Object);

	Audio->AddSoundWave(TEXT("Heal"), TEXT("/Game/Assets/Sound/Common/Common_Heal.Common_Heal"));

	RootComponent->SetWorldScale3D(FVector{ FMath::FRandRange(0.1f, 0.5f) });
	Light->Intensity = 500000.0f;
	Light->LightColor = FColor{ 255, 0, 0 };
	Light->AttenuationRadius = RootComponent->GetComponentScale().X * 200.0f;
	Light->SourceRadius = RootComponent->GetComponentScale().X * 200.0f;
	Light->SourceLength = 3500.0f;
	BoundBox->OnComponentBeginOverlap.AddDynamic(this, &ATTHPBead::OnOverlapBegin);
	BoundBox->SetSimulatePhysics(false);
	MoveSpeed = 7500.0f;
}

void ATTHPBead::BeginPlay()
{
	Super::BeginPlay();
	
	BoundBox->SetCollisionProfileName(TEXT("Item"));
	SpawnedTime = GetWorld()->GetTimeSeconds();
}

void ATTHPBead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const auto Player{ Cast<ATTPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) };
	
	if (Player && GetWorld()->GetTimeSeconds() - SpawnedTime < 3.0f)
	{
		float NewLocationZ{ GetActorLocation().Z + FMath::Sin(Angle += DeltaTime * 5.0f) };
		SetActorLocation(FVector{ GetActorLocation().X, GetActorLocation().Y, NewLocationZ });
	}
	else
	{
		FVector MoveDirection{ (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal() };
		SetActorLocation(GetActorLocation() + MoveDirection *  MoveSpeed * DeltaTime);
	}
}

void ATTHPBead::OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Player{ Cast<ATTPlayer>(OtherActor) };

	if (Player)
	{
		TTCHECK(Player->CharacterStat);
		const auto PlayerStat{ Player->CharacterStat };

		PlayerStat->SetHP(PlayerStat->GetHP() + RootComponent->GetComponentScale().X * 200.0f);
		Audio->PlaySoundWave2D(TEXT("Heal"));

		Destroy();
	}
}