#include "TTManual.h"
#include "TTPlayerController.h"
#include "Components/WidgetComponent.h"

ATTManual::ATTManual()
{
	PrimaryActorTick.bCanEverTick = true;

	Plain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PLAIN"));
	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOUNDBOX"));

	RootComponent = Plain;
	BoundBox->SetupAttachment(RootComponent);

	Plain->SetCollisionProfileName(TEXT("NoCollision"));
	BoundBox->SetCollisionProfileName(TEXT("Item"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLAIN{ TEXT("/Engine/BasicShapes/Plane.Plane") };
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_BUBBLE{ TEXT("/Game/Assets/UI/Material/M_TextBubble.M_TextBubble") };
	if (SM_PLAIN.Succeeded()) Plain->SetStaticMesh(SM_PLAIN.Object);
	if (M_BUBBLE.Succeeded()) Plain->SetMaterial(0, M_BUBBLE.Object);

	BoundBox->SetRelativeScale3D(FVector{ 2.0f, 2.0f, 2.0f });
	BoundBox->OnComponentBeginOverlap.AddDynamic(this, &ATTManual::OnOverlapBegin);
	BoundBox->OnComponentEndOverlap.AddDynamic(this, &ATTManual::OnOverlapEnd);
	BoundBox->SetSimulatePhysics(false);
}

void ATTManual::BeginPlay()
{
	Super::BeginPlay();

	TTPlayerController = Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController());
}

void ATTManual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector& CameraForwardVector{ UGameplayStatics::GetPlayerCameraManager(this, 0)->GetActorForwardVector() };
	const FRotator& TargetRot{ FRotationMatrix::MakeFromX(-CameraForwardVector.GetSafeNormal2D()).Rotator() };
	SetActorRotation(TargetRot + FRotator{ 0.0f, -90.0f, 90.0f });
}

void ATTManual::OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	TTPlayerController->GetUIManual()->SetVisibility(ESlateVisibility::Visible);
}

void ATTManual::OnOverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	TTPlayerController->GetUIManual()->SetVisibility(ESlateVisibility::Hidden);
}

