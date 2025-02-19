#include "TTUIMapButton.h"
#include "TTPlayer.h"
UTTUIMapButton::UTTUIMapButton()
{
	SetRenderTransformPivot(FVector2D{ 0.0f, 0.0f });
	SetIsEnabled(false);
}

void UTTUIMapButton::OnClickEvent()
{
	SetEnableChilds(true);
	if (Parent)
		Parent->SetEnableChilds(false);
	SetIsEnabled(false);

	Cast<ATTPlayer>(UGameplayStatics::GetPlayerCharacter(WorldContext, 0))->SetUIMapOpenForced(false);
}

void UTTUIMapButton::SetButtonType(USlateWidgetStyleAsset* MapWidgetStyle)
{
	SButton::FArguments Default{};
	Default.ButtonStyle(MapWidgetStyle);
	WidgetStyle = *Default._ButtonStyle;
}

void UTTUIMapButton::SetOriginPosition(const FVector2D& Coordinate)
{
	OriginPosition = Coordinate;
}

void UTTUIMapButton::RegistChild(UTTUIMapButton* Child)
{
	Childs.Add(Child);
}

void UTTUIMapButton::SetParent(UTTUIMapButton* Button)
{
	Parent = Button;
}

void UTTUIMapButton::SetWorldContext(UWorld* World)
{
	WorldContext = World;
}

void UTTUIMapButton::SetEnableChilds(bool bFlag)
{
	for (auto& Child : Childs)
		Child->SetIsEnabled(bFlag);
}