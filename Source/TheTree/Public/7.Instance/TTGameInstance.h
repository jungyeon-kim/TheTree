#pragma once

#include "TheTree.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "TTGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FTTCharacterData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName ObjectName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxSta;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Def;
public:
	FTTCharacterData() : ObjectName(NAME_None), MaxHP(0.0f), MaxSta(0.0f), Atk(0.0f), Def(0.0f) {}
};

UCLASS()
class THETREE_API UTTGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UDataTable* TTCharacterTable;
public:
	UTTGameInstance();

	virtual void Init() override;

	FTTCharacterData* GetTTCharacterData(FName ObjectName);
};
