#pragma once

#include "TheTree.h"
#include "GameFramework/PlayerController.h"
#include "TTPlayerController.generated.h"

UCLASS()
class THETREE_API ATTPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class UTTUIPlayerInGame* TTUIPlayerInGame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTTUIPlayerInGame> TTUIPlayerInGameClass;
protected:
	virtual void BeginPlay() override;
public:
	ATTPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	class UTTUIPlayerInGame* GetUIPlayerInGame() const;

	void SwapDebugMode();
};
