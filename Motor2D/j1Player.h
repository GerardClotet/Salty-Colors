#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "p2Animation.h"


class j1Player : public j1Entity
{
private:
public:
	j1Player(ENTITY_TYPE type, pugi::xml_node, fPoint position, p2SString id, int clone_number);
	~j1Player();
};

#endif