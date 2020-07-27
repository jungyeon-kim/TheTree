#include "TTRecoveryNPC.h"
#include "TTAudioComponent.h"
#include "TTPlayerController.h"
#include "TTUIRecovery.h"

ATTRecoveryNPC::ATTRecoveryNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BOUNDBOX"));
	Audio = CreateDefaultSubobject<UTTAudioComponent>(TEXT("AUDIO"));

	RootComponent = GetCapsuleComponent();
	BoundBox->SetupAttachment(RootComponent);
	Audio->SetupAttachment(RootComponent);

	BoundBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NPC"));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NPC{ TEXT("/Game/Assets/Character/NPC/SK_Highelf_NPC.SK_Highelf_NPC") };
	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM{ TEXT("/Game/Blueprints/Animation/NPC/HighElfNPCAnimBlueprint.HighElfNPCAnimBlueprint_C") };
	if (SK_NPC.Succeeded()) GetMesh()->SetSkeletalMesh(SK_NPC.Object);
	if (NPC_ANIM.Succeeded()) GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);

	Audio->AddSoundCue(TEXT("Greet"), TEXT("/Game/Assets/Sound/NPC/Highelf_NPC_GreetCue.Highelf_NPC_GreetCue"));
	Audio->AddSoundCue(TEXT("Bye"), TEXT("/Game/Assets/Sound/NPC/Highelf_NPC_ByeCue.Highelf_NPC_ByeCue"));

	GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, -88.0f });
	BoundBox->AddRelativeLocation(FVector{ 0.0f, 0.0f, 100.0f });
	BoundBox->SetWorldScale3D(FVector{ 5.0f, 5.0f, 5.0f });
	BoundBox->SetSimulatePhysics(false);
}

void ATTRecoveryNPC::BeginPlay()
{
	Super::BeginPlay();

	TTPlayerController = Cast<ATTPlayerController>(GetWorld()->GetFirstPlayerController());

	BoundBox->OnComponentBeginOverlap.AddDynamic(this, &ATTRecoveryNPC::OnOverlapBegin);
	BoundBox->OnComponentEndOverlap.AddDynamic(this, &ATTRecoveryNPC::OnOverlapEnd);
}

void ATTRecoveryNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATTRecoveryNPC::OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	Audio->PlaySoundCue2D(TEXT("Greet"));

	TTPlayerController->GetUIRecovery()->UpdatePlayerGold();

	TTPlayerController->bShowMouseCursor = true;
	TTPlayerController->SetIgnoreLookInput(true);
	TTPlayerController->SetInputMode(FInputModeGameAndUI{});
	TTPlayerController->GetUIRecovery()->SetVisibility(ESlateVisibility::Visible);
}

void ATTRecoveryNPC::OnOverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0)) return;

	Audio->PlaySoundCue2D(TEXT("Bye"));

	TTPlayerController->bShowMouseCursor = false;
	TTPlayerController->SetIgnoreLookInput(false);
	TTPlayerController->SetInputMode(FInputModeGameOnly{});
	TTPlayerController->GetUIRecovery()->SetVisibility(ESlateVisibility::Hidden);
}
