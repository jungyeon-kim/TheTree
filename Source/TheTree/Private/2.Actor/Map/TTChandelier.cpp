#include "TTChandelier.h"

ATTChandelier::ATTChandelier()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ChandelierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CHANDELIER"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CHANDELIER
	{TEXT("/Game/Assets/StaticMesh/Chandelier/SM_Chandelier.SM_Chandelier")};

	if (SM_CHANDELIER.Succeeded())
		ChandelierMesh->SetStaticMesh(SM_CHANDELIER.Object);

	ChandelierMesh->SetMobility(EComponentMobility::Static);
	ChandelierMesh->SetWorldScale3D(FVector{ 1.5f,1.5f,1.0f });
	RootComponent = ChandelierMesh;
	
	ChandelierLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CHANDELIERLIGHT"));
	ChandelierLight->Intensity = 75.0f;
	ChandelierLight->LightColor = FColor{ 255,173,93 };
	ChandelierLight->AttenuationRadius = 7500.0f;
	ChandelierLight->SourceRadius = 35000.0f;
	ChandelierLight->SourceLength = 35000.0f;
	ChandelierLight->ShadowBias = 0.27619f;
	ChandelierLight->VolumetricScatteringIntensity = 0.2f;
	ChandelierLight->bUseInverseSquaredFalloff = false;
	ChandelierLight->CastShadows = false;

	static ConstructorHelpers::FObjectFinder<UMaterial> M_LIGHTFUNCTION
	{ TEXT("/Game/Assets/StaticMesh/Torch/M_Torch_Flicker_Light_Function.M_Torch_Flicker_Light_Function") };
	if (M_LIGHTFUNCTION.Succeeded())
		ChandelierLight->LightFunctionMaterial = M_LIGHTFUNCTION.Object;

	ChandelierLight->SetMobility(EComponentMobility::Movable);
	ChandelierLight->SetRelativeLocation(FVector{ 0.0f,0.0f,310.0f });
	ChandelierLight->SetupAttachment(RootComponent);
}

void ATTChandelier::BeginPlay()
{
	Super::BeginPlay();
}

void ATTChandelier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

