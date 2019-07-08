#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"


class j1Player : public j1Entity
{
private:
public:
	j1Player(iPoint position);
	~j1Player();

	bool Start();
	bool Update(float dt);
	void Draw();
	bool CleanUp();
public:
	iPoint position;
};

#endif