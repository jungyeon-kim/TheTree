#pragma once

#include "TheTree.h"
#include "Blueprint/UserWidget.h"
#include "TTUIMap.generated.h"

UCLASS()
class THETREE_API UTTUIMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	enum class EButtonType
	{
		MONSTER,
		SHELTER,
		TROOPER,
		DURION,
		SHOP,
		CURRENT
	};

	UPROPERTY()
	TArray<class UButton*> ButtonCluster;

	UPROPERTY()
	TArray<class USlateWidgetStyleAsset*> WidgetCluster;

	UPROPERTY()
	class UCanvasPanel* Panel;

	/*
	 * Generate UI Map
	 *
	 * @param StartX : X Coordinate On 2nd Dimension 
	 * @param StartY : Y Coordinate On 2nd Dimension => On the Viewport
	 */
	void GenerateMapRecursive(int Layer, int StartX, int StartY, int EndX, int EndY);
	void GenerateMapRecursiveImpl(int GenerateLayer, int StartX, int StartY);
	void CreateButton(const EButtonType& ButtonType, int PosX, int PosY);
};
