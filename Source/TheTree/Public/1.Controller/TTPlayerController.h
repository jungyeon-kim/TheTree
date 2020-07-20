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
	UPROPERTY()
	class UTTUIPlayerStatus* TTUIPlayerStatus;
	UPROPERTY()
	class UTTUIReinforce* TTUIReinforce;
	UPROPERTY()
	class UTTUIMap* TTUIMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTTUIPlayerInGame> TTUIPlayerInGameClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTTUIPlayerStatus> TTUIPlayerStatusClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTTUIReinforce> TTUIReinforceClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTTUIMap> TTUIMapClass;

protected:
	virtual void BeginPlay() override;
public:
	ATTPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	class UTTUIPlayerInGame* GetUIPlayerInGame() const;
	class UTTUIPlayerStatus* GetUIPlayerStatus() const;
	class UTTUIReinforce* GetUIReinforce() const;
	class UTTUIMap* GetUIMap() const;
	void SetUIPlayerInGame(class UTTCharacterStatComponent* NewCharacterStat);
	void SetUIPlayerStatus(class UTTCharacterStatComponent* NewCharacterStat);
	void SetUIReinforce(class UTTCharacterStatComponent* NewCharacterStat);
	void SetUIMap();

	void SwapDebugMode();
};
