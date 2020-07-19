#include "TTUIMap.h"
#include "TTUIMapButton.h"
void UTTUIMap::NativeConstruct()
{
	Super::NativeConstruct();
	UTTUIMapButton* BaseButton = NewObject<UTTUIMapButton>(this);
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_0")));

	BaseButton->SetRenderScale(FVector2D{ 1.0f, 2.0f });
	BaseButton->SetRenderTranslation(FVector2D{ 960.0f, 540.0f });
	if (CanvasPanel)
		CanvasPanel->AddChild(BaseButton);
}

bool UTTUIMap::Initialize()
{
	Super::Initialize();
	return true;
}

void UTTUIMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}