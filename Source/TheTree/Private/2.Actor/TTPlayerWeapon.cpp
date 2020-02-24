#include "TTPlayerWeapon.h"

ATTPlayerWeapon::ATTPlayerWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

	RootComponent = Weapon;
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON{ TEXT("/Game/Assets/StaticMesh/Weapon/SM_Player_Weapon.SM_Player_Weapon") };
	if (SM_WEAPON.Succeeded()) Weapon->SetStaticMesh(SM_WEAPON.Object);
}

void ATTPlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ATTPlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

