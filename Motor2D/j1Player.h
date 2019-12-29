#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include <vector>
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

	void IdleUpdate();
	void MovingUpdate();
	void JumpingUpdate();
	void GodUpdate();
	void Die();
	void DashUpdate();
	void BounceUpdate();
	void ResetPlayer();
	bool AwakeAttributes(pugi::xml_node config);
	void CheckWalkSound();
	void MovingActPool();
	void IdleActPool();
	bool JumpActPool();
	void BounceActPool();
	bool Bounce();
	void Ground();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;



	
public:

	
	float spriteIncrease = 1.5f;
	j1PerfTimer stepSFXTimer;
	bool startMove = false;
	bool lockInput = false;
	bool ready_toBounce_left = false;
	bool ready_toBounce_right = false;

	

private:
	bool startDash = false;
	int dashes = 1;
	float distance;
	float init_distance;
	float previous_pos;
	
	Animation player_IDLE;
	Animation player_RUN;
	Animation player_JUMP;
	Animation player_FALL;
	Animation player_MOMENTUM;
	Animation player_DASH;
	Animation player_WALL;
	Animation player_BOUNCE;




};

#endif