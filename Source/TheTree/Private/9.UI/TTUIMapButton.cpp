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