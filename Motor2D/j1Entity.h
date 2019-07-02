#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__


#include "p2Animation.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"

#include "SDL_image/include/SDL_image.h"


enum ENTITY_TYPE
{
	NO_TYPE,
	PLAYER,
	MAX
};

class j1Entity
{
public:
	j1Entity(ENTITY_TYPE type, float posX, float posY);
	virtual ~j1Entity();

	virtual bool Start();
	// core loops ---------------
	virtual bool PreUpdate();
	virtual bool Update();
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual void Draw();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	fPoint position;
	bool to_delete = false;
	SDL_Rect entityRect;
	SDL_Texture* entityTex;
	ENTITY_TYPE type;
	Animation* currentAnimation;
};
#endif