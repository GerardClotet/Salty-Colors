#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Collision.h"
#define MAX_DASHES 1


class j1Player : public j1Entity
{

	
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
	void DashUpdate();
	void ResetPlayer();
	bool LoadAttributes(pugi::xml_node config);
public:

	SDL_Rect animation_Coll;
	float spriteIncrease = 1.5f;
	j1PerfTimer stepSFXTimer;
	bool startMove = false;
	bool lockInput = false;

private:
	bool startDash = false;
	int dashes = 1;
	
};

#endif