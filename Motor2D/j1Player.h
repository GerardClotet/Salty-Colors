#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Collision.h"

//enum  PlayerState {
//	NO_STATE =-1,
//	IDLE,
//	MOVING,
//	JUMPING,
//	DEAD,
//	GOD,
//	MAX
//};
class j1Player : public j1Entity
{
private:

	//void MovingUpdate();
	//void IdleUpdate();
	//void JumpingUpdate();
	//void Jump();
	
public:
	j1Player(iPoint position);
	~j1Player();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	void Draw();
	bool CleanUp();
	void SetPos(iPoint pos);

	void IdleUpdate();
	void MovingUpdate();
	void JumpingUpdate();
	void GodUpdate();
	void Die();

	bool LoadAttributes(pugi::xml_node config);
public:

	//PlayerState state = IDLE;
	SDL_Rect animation_Coll;
};

#endif