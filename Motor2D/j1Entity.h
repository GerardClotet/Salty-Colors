#ifndef __J1ENTITY_H__
#define __J1ENTITY_H__


#include "p2Animation.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Textures.h"
#include "j1App.h"

#include "SDL_image/include/SDL_image.h"


enum  ENTITY_TYPE
{
	NO_TYPE,
	ENT_PLAYER,
	ENT_MAX
};

class j1Entity 
{
public:
	j1Entity(ENTITY_TYPE type, iPoint position);
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
	iPoint position;
	bool to_delete = false;
	SDL_Rect entityRect;
	SDL_Texture* entityTex;
	ENTITY_TYPE type;
	Animation currentAnimation;

	float movment_speed = 0.0F;
	float jump_speed = 0.0F;
	float gravity = 0.0F;
	float acceleration = 0.0F;
	float fall_speed = 0.0F;
	fPoint target_speed = { 0.0F,0.0F };
};
#endif