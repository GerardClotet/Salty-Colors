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

	bool moving_right = false;
	bool moving_left = false;
	bool jump = false;
	bool chase = false;
	int total_jumps = 0;

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
	



public:
	float spriteIncrease = 1.5f;

	p2DynArray<iPoint> current_path;

	iPoint position;

	Animation e_test_IDLE;

};


#endif