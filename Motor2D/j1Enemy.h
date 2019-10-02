#ifndef __J1ENEMY_H__
#define __J1ENEMY_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "j1Collision.h"



#define MAX_JUMPS 1


class j1Enemy : public j1Entity
{
public:

	SDL_Rect animation_Coll;

	j1Enemy(iPoint position);
	~j1Enemy();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();


	
};


#endif