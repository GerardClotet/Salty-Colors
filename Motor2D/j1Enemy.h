#ifndef __J1ENEMY_H__
#define __J1ENEMY_H__

#include "j1Entity.h"




#define MAX_JUMPS 1

enum class  ENEMY_TYPE
{
	NO_TYPE = -1,
	ENEMY_TEST,
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


public:
	float spriteIncrease = 1.5f;


	iPoint position;

	Animation e_test_IDLE;

};


#endif