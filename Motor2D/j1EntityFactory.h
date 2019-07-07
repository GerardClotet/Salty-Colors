#ifndef _J1ENTITYFACTORY_H_
#define _J1ENTITYFACTORY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include <vector>
#include <list>


class j1EntityFactory : public j1Module //ENTITY MANAGER AND CREATOR
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	j1Player* CreatePlayer(iPoint pos);
	//CreateEntities();
public:

	std::list<j1Entity*>	entities;

	j1Player*				player = nullptr;
	SDL_Texture* playerSprite = nullptr;
	Animation player_IDLE;
	Animation player_RUN;
	std::string sprite_route;

private:
	std::vector<j1Entity*>	draw_entities;

};
#endif // !_J1ENTITYFACTORY_H_
