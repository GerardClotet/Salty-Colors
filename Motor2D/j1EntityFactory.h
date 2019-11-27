#ifndef _J1ENTITYFACTORY_H_
#define _J1ENTITYFACTORY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Enemy.h"
#include "TestEnemy.h"
#include "FlyingEnemy.h"
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
//	j1Enemy* CreateEnemy(iPoint pos, ENTITY_TYPE type /*!!!! NEED TO CHANGE THIS ENUM FOR 1 exclusivly for enemies*/);
	j1Enemy* CreateEnemy(iPoint pos, ENEMY_TYPE type);
	bool GetPlayerState()const;
	//bool GetEnemyState();

	bool draw_path = false;
	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);
	void DeleteAllEnemies();
	//CreateEntities();
public:
	
	std::list<j1Entity*>	entities;

	j1Player*	player = nullptr;
	j1Enemy*	enemy = nullptr;


	int maxframes = 4;
	bool playerActive = false;


private:
	std::vector<j1Entity*>	draw_entities;

};
#endif // !_J1ENTITYFACTORY_H_
