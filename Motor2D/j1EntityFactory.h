#ifndef _J1ENTITYFACTORY_H_
#define _J1ENTITYFACTORY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Enemy.h"

#include <vector>
#include <list>
#define MAX_LIVES 3


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

	
	bool GetPlayerState()const;

	j1Entity* CreateEntity(iPoint pos, E_TYPE type);
	bool draw_path = false;
	bool Save(pugi::xml_node&)const;

	bool Load(pugi::xml_node&);
	void DeleteAllEnemies();
	void DeleteEntity(j1Entity* entity);
	void DeleteAllCollectables();
	void CheckifCoinsWereTaken();

	bool IsTaken(int id);

	void SetCoinID(int id);
	std::vector<int> GetCoinVec()const;
	void ClearCoinVec();
public:
	
	std::list<j1Entity*>	entities;

	j1Player*	player = nullptr;


	int maxframes = 4;
	bool playerActive = false;
	int coins = 0;
	int lives = MAX_LIVES;


private:
	std::vector<j1Entity*>	draw_entities;
	std::vector<int> coins_ids;

};
#endif // !_J1ENTITYFACTORY_H_
