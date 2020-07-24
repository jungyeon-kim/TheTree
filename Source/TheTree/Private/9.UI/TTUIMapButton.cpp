#include "TTUIMapButton.h"

UTTUIMapButton::UTTUIMapButton()
{

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