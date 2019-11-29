#ifndef __J1ENEMY_H__
#define __J1ENEMY_H__

#include "j1Entity.h"
#include "p2DynArray.h"



#define MAX_JUMPS 1

enum class  ENEMY_TYPE
{
	NO_TYPE = -1,
	ENEMY_TEST,
	ENEMY_FLYING,
	MAX
};

class j1Enemy : public j1Entity
{
public:



	//SDL_Rect animation_Coll;

	j1Enemy(iPoint position);
	virtual ~j1Enemy();

	virtual bool Start();
	virtual	bool PreUpdate();
	virtual	bool Update(float dt);
	virtual	bool CleanUp();
	virtual	void Draw();

	void MovX();
	void MovY();
	
	virtual bool GetPath();
	virtual void ResetPathfindingVariables();


	virtual void DrawPath();
	virtual void PathfindingUpdate();
	virtual void PathfindingPreupdate();
	virtual void PathfindX();
	virtual void PathfindY();

	bool CheckifHasReachedDesPos(int des, int current);


public:
	float spriteIncrease = 1.5f;

	p2DynArray<iPoint> current_path;

	iPoint position;

	Animation e_test_IDLE;


	float chase_distance = 1000.0F;
	int current_destination = 0;
	int previous_destination = 0;
	int next_destination = -1;
	int jump_height = 2;
	bool reached_X = false;
	bool reached_Y = false;
	bool current_is_grounded = false;
	iPoint destination = { -1, -1 };

	bool moving_right = false;
	bool moving_left = false;
	bool moving_down = false;
	bool jump = false;
	bool chase = false;
	int total_jumps = 0;
	int previous_pos = -1;

};


#endif