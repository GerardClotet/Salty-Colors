#ifndef _J1ENTITYFACTORY_H_
#define _J1ENTITYFACTORY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Enemy.h"
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
	j1Enemy* CreateEnemy(iPoint pos);
	bool GetPlayerState();
	bool GetEnemyState();

	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);
	//CreateEntities();
public:
	
	std::list<j1Entity*>	entities;

	j1Player*	player = nullptr;
	j1Enemy*	enemy = nullptr;
	SDL_Texture* playerSprite = nullptr;
	Animation player_IDLE;
	Animation player_RUN;
	Animation player_JUMP;
	Animation player_FALL;
	Animation player_MOMENTUM;
	Animation player_DASH;
	Animation player_WALL;
	Animation player_BOUNCE;
	std::string sprite_route;

	int maxframes = 4;
	bool playerActive = false;

private:
	std::vector<j1Entity*>	draw_entities;

};
#endif // !_J1ENTITYFACTORY_H_
