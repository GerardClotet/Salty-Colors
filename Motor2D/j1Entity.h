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
	ENT_ENEMY,
	ENT_FLYING,
	ENT_MAX
};
enum  PlayerState {
	NO_STATE = -1,
	IDLE,
	MOVING,
	JUMPING,
	DEAD,
	DASH,
	BOUNCE,
	GOD,

	MAX
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
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	virtual iPoint GetPosition();
	void MovX();

	void MovY();

	/*ENTITY_TYPE type;*/

public:
	Collider* collider;

	iPoint position;
	bool to_delete = false;
	SDL_Rect entityRect;
	SDL_Texture* entityTex;
	ENTITY_TYPE type;
	SDL_Rect currentAnimation;

	float movement_speed;
	float jump_speed;
	float gravity;
	float acceleration;
	float fall_speed;
	float bounce_speed;
	fPoint target_speed = { 0.0F,0.0F };

	fPoint velocity = { 0.0F, 0.0F };
	fPoint pivot = { 0.0F, 0.0F };
	bool ignore_platforms = false;
	bool flipX = false;
	bool in_contact = false;
	bool is_grounded = false;


	PlayerState state = IDLE;

	float norm_moves;
	
};
#endif