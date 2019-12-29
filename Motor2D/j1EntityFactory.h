#ifndef _J1ENTITYFACTORY_H_
#define _J1ENTITYFACTORY_H_

#include "j1Module.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Enemy.h"

#include <vector>
#include <list>
#define MAX_LIVES 3

struct SDL_Texture;
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

	void IncreaseLifesBy(int inc);
	void SetLivesTo(int lives);
	int GetCurrentLives()const;

	std::string GetCoinsToString()const;
	std::string GetLivesToString()const;
public:
	
	std::list<j1Entity*>	entities;

	j1Player*	player = nullptr;

	//Entities Textures
	SDL_Texture* coin_tex = nullptr;
	SDL_Texture* walk_enemy_tex = nullptr;
	SDL_Texture* fly_enemy_tex = nullptr;
	SDL_Texture* player_tex = nullptr;

	int maxframes = 4;
	bool playerActive = false;
	int coins = 0;
	int reward_counter = 0;

	

	iPoint temp_pos = { 0,0 };
	bool previous_play = false;
private:
	std::vector<j1Entity*>	draw_entities;
	std::vector<int> coins_ids;

	int lives = MAX_LIVES;

};
#endif // !_J1ENTITYFACTORY_H_
