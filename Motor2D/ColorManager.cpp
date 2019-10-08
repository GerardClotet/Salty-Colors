#include "ColorManager.h"


SDL_Color ColorManager::GetColor(){	return color_sdl;}

Uint8 ColorManager::GetRed(){	return color_sdl.r;}

Uint8 ColorManager::GetBlue(){	return color_sdl.b;}

Uint8 ColorManager::GetGreen(){	return  color_sdl.g;}

void ColorManager::IncGreenTo_time(Uint8 value, float time)
{
	time = time_g;

	ManageGreenIncrease(value);
}

void ColorManager::ManageGreenIncrease(Uint8 value)
{
	int increase = (color_sdl.g - value) / time_g;
	color_sdl.g +=  increase;
}

void ColorManager::SetColor(SDL_Color color) { color_sdl = color; }

void ColorManager::SetRed(Uint8 r){color_sdl.r = r;}

void ColorManager::SetGreen(Uint8 g){color_sdl.g = g;}

void ColorManager::SetBlue(Uint8 b){color_sdl.b = b;}
