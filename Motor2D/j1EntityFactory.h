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
	bool GetPlayerState();

	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);
	//CreateEntities();
public:
	
	std::list<j1Entity*>	entities;

	j1Player*				player = nullptr;
	SDL_Texture* playerSprite = nullptr;
	Animation player_IDLE;
	Animation player_RUN;
	Animation player_JUMP;
	Animation player_FALL;
	Animation player_MOMENTUM;

	std::string sprite_route;
	int maxframes = 4;
	bool playerActive = false;

private:
	std::vector<j1Entity*>	draw_entities;

};
#endif // !_J1ENTITYFACTORY_H_
