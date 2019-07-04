#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__


#include "p2Animation.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"

#include "SDL_image/include/SDL_image.h"


enum  ENTITY_TYPE
{
	NO_TYPE,
	ENT_PLAYER,
	ENT_MAX
};

class j1Entity
{
protected:
	p2SString sprite_route;
	SDL_Rect entityRect;
	SDL_Texture* entityTex;
	ENTITY_TYPE type;
	Animation* currentAnimation;

	virtual void LoadAnimations(pugi::xml_node conf);
public:
	j1Entity(ENTITY_TYPE type,pugi::xml_node config, fPoint position, p2SString id);
	virtual ~j1Entity();

	virtual bool Start();
	// core loops ---------------
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual void Draw();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	fPoint position = {0.0F,0.0F};
	bool to_delete = false;

};
#endif