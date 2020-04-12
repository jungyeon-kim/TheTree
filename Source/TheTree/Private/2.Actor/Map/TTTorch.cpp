#include "TTTorch.h"

ATTTorch::ATTTorch()
{
	PrimaryActorTick.bCanEverTick = false;

	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TORCHMESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TORCHMESH
	{ TEXT("/Game/Assets/StaticMesh/Torch/SM_Torch.SM_Torch") };

	if (SM_TORCHMESH.Succeeded())
		TorchMesh->SetStaticMesh(SM_TORCHMESH.Object);
	
	RootComponent = TorchMesh;
	TorchMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.3f));

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TORCHLIGHT"));
	TorchLight->SetMobility(EComponentMobility::Movable);
	TorchLight->Intensity = 300.0f;
	TorchLight->LightColor = FColor{ 255,173,93 };
	TorchLight->AttenuationRadius = 2000.0f;
	TorchLight->SourceRadius = 5000.0f;
	TorchLight->SourceLength = 50000.0f;
	TorchLight->CastShadows = false;
	TorchLight->VolumetricScatteringIntensity = 0.2f;
	TorchLight->bUseInverseSquaredFalloff = false;
	TorchLight->LightFalloffExponent = 4.0f;
	TorchLight->ShadowBias = 0.27619f;
	
	TorchLight->SetupAttachment(RootComponent);
	TorchLight->SetRelativeLocation(FVector{0.0f, 0.0f, -230.0f});
	
	static ConstructorHelpers::FObjectFinder<UMaterial> M_LIGHTFUNCTION
	{ TEXT("/Game/Assets/StaticMesh/Torch/M_Torch_Flicker_Light_Function.M_Torch_Flicker_Light_Function") };
	if (M_LIGHTFUNCTION.Succeeded())
		TorchLight->LightFunctionMaterial = M_LIGHTFUNCTION.Object;

	TorchParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TORCHPARTICLE"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_TORCHPARTICLE
	{ TEXT("/Game/Assets/StaticMesh/Torch/P_Torch_Fire.P_Torch_Fire") };

	if (P_TORCHPARTICLE.Succeeded())
		TorchParticle->SetTemplate(P_TORCHPARTICLE.Object);

	TorchParticle->SetupAttachment(RootComponent);
	TorchParticle->SetRelativeLocation(FVector{ -5.0f, 0.0f, 1920.0f });
	TorchParticle->SetWorldScale3D(FVector{ 4.0f,4.0f,2.0f });
}

void ATTTorch::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATTTorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

