#ifndef  _COLOR_MANAGER_H
#define _COLOR_MANAGER_H
#include "j1Render.h"
class ColorManager
{
public:
	ColorManager() {};

	~ColorManager() {};

	void SetColor(SDL_Color color);
	void SetRed(Uint8 r);
	void SetGreen(Uint8 g);
	void SetBlue(Uint8 b);
	
	SDL_Color GetColor();
	Uint8 GetRed();
	Uint8 GetBlue();
	Uint8 GetGreen();
	void IncGreenTo_time(Uint8 value, float time);

private:
	void ManageGreenIncrease(Uint8 value);
	float time_g;
public:
	
	SDL_Color color_sdl;
};
#endif // ! _COLOR_MANAGER_H
