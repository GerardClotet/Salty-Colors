#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Timer.h"


#define BOTTOM_OFFSET 900
#define TOP_OFFSET 100
#define SIDE_OFFSET 600
#define TRAVEL_SPEED 0.04
struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool SwapMap();

	bool Loadlvl(int lvl);

	void TriggerColl();

	void ReLoadLevel();
	bool DoViewportResize();


	SDL_Rect RezieView(SDL_Rect viewport,bool camera);
	j1Timer resizeTimer;

	void PartyMaker();

	int LerpLabel(int v0, int v1, float t);
	void CameraFollowingUpdate();
public:
	int currentMap;
	float fadeTime;
	std::list<std::string> map_names;
	std::string actualMap;
	int maptoReset=0;

	/*---SFX---*/

	unsigned int stepSFX;
	unsigned int landSFX;
	unsigned int jumpSFX;
	unsigned int endLvlSFX;
	unsigned int dashSFX;
	unsigned int bounceSFX;
private:
	bool viewportResize = false;
	bool incR = true;
	bool incG = true;
	bool incB = true;
	bool stopX = false;
	bool stopY = false;

	Uint8 red=0;
	Uint8 green=125;
	Uint8 blue=255;

};

#endif // __j1SCENE_H__