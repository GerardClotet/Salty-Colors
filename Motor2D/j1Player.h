#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Collision.h"
#define MAX_DASHES 1
#define DASH_DISTANCE 150
#define STEP_TIME 250.0F
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
	void BounceUpdate();
	void ResetPlayer();
	bool LoadAttributes(pugi::xml_node config);
public:

	SDL_Rect animation_Coll;
	float spriteIncrease = 1.5f;
	j1PerfTimer stepSFXTimer;
	bool startMove = false;
	bool lockInput = false;
	bool ready_toBounce_left = false;
	bool ready_toBounce_right = false;
	void CheckWalkSound();
	void MovingActPool();
	void IdleActPool();
	void JumpActPool();
	void BounceActPool();
	void Bounce();
	void Ground();
private:
	bool startDash = false;
	int dashes = 1;
	float distance;
	float init_distance;
	float previous_pos;
	

public:

	Animation player_IDLE;
	Animation player_RUN;
	Animation player_JUMP;
	Animation player_FALL;
	Animation player_MOMENTUM;
	Animation player_DASH;
	Animation player_WALL;
	Animation player_BOUNCE;
	std::string sprite_route;
};

#endif