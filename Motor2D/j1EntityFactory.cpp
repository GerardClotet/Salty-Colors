#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include <algorithm>

j1EntityFactory::j1EntityFactory()
{
	name.assign("entityFactory");
	
}

j1EntityFactory::~j1EntityFactory()
{
}

bool j1EntityFactory::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

bool j1EntityFactory::Start()
{
	std::list<j1Entity*>::iterator iter = entities.begin();

	for (; iter != entities.end(); ++iter)
	{
		if ((*iter) != nullptr)
			(*iter)->Start();

	}



	return true;
}

bool j1EntityFactory::PreUpdate()
{
	bool ret = true;

	// logic / collisions
	std::list<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		if ((*item) != nullptr)
		{
			ret = ((*item)->PreUpdate());
		}
	}

	return true;
}

bool j1EntityFactory::Update(float dt)
{
	bool ret = true;
	std::list<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end();)
	{
		if ((*item) != nullptr)
		{

			if (!(*item)->to_delete)
			{
				ret = (*item)->Update(dt);

				++item;

			}
			else
			{
				//if ((*item)->type == ENTITY_TYPE::ENT_PLAYER)
				//{
				//	//player = nullptr;
				//	LOG("");
				//}

				(*item)->CleanUp();
				delete(*item);
				(*item) = nullptr;
				item = entities.erase(item);
			}
		}
		else
		{
			item = entities.erase(item);

		}
	}

	return ret;
}

bool j1EntityFactory::PostUpdate()
{
	std::list<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->PostUpdate();
	}

	std::vector<j1Entity*>::iterator drawItem = draw_entities.begin();
	for (; drawItem != draw_entities.end(); ++drawItem)
	{
		(*drawItem)->Draw();

		//here can draw debug for entitys
	}

	draw_entities.clear();

	return true;


}

bool j1EntityFactory::CleanUp()

{
	bool ret = true;

	std::list<j1Entity*>::reverse_iterator entitiesItem = entities.rbegin();
	while (entitiesItem != entities.rend())
	{
		if ((*entitiesItem) != nullptr)
		{
			(*entitiesItem)->CleanUp();
			RELEASE(*entitiesItem);
			*entitiesItem = nullptr;
		}
		++entitiesItem;

	}
	entities.clear();

	draw_entities.clear();


	//unload entitiesTex and player
	return ret;
}

j1Player* j1EntityFactory::CreatePlayer(iPoint pos)
{
	player = new j1Player(pos);

	if (player != nullptr)
	{
		entities.push_back(player);
		playerActive = true;
		return player;
	}

	LOG("Failed to create player");
	return nullptr;
}

j1Enemy* j1EntityFactory::CreateEnemy(iPoint pos)
{
	enemy = new j1Enemy(pos);

	if (enemy != nullptr)
	{
		entities.push_back(enemy);
		return enemy;
	}

	LOG("Failed to create player");
	return enemy;
}



bool j1EntityFactory::GetPlayerState()
{
	return playerActive;
}

bool j1EntityFactory::Save(pugi::xml_node& data) const
{
	pugi::xml_node Ppos;
	

	Ppos = data.append_child("Player");

	Ppos.append_attribute("x") = player->position.x;
	Ppos.append_attribute("y") = player->position.y;


	Ppos.append_child("velocity").append_attribute("x") = player->velocity.x;
	Ppos.append_child("velocity").append_attribute("y") = player->velocity.y;

	Ppos.append_child("target_speed").append_attribute("x") = player->target_speed.x;
	Ppos.append_child("target_speed").append_attribute("y") = player->target_speed.y;


	Ppos.append_child("state").append_attribute("value") = player->state != DEAD ? (int)player->state : (int)IDLE;
	Ppos.append_child("is_grounded").append_attribute("value") = player->is_grounded;
	Ppos.append_child("flipX").append_attribute("value") = player->flipX;

	return true;
}

bool j1EntityFactory::Load(pugi::xml_node& data)
{
	
	player->position.x = data.child("Player").attribute("x").as_int();
	player->position.y = data.child("Player").attribute("y").as_int();


	player->velocity.x = data.child("Player").child("velocity").attribute("x").as_float();
	player->velocity.y = data.child("Player").child("velocity").attribute("y").as_float();

	player->target_speed.x = data.child("Player").child("target_speed").attribute("x").as_float();
	player->target_speed.y = data.child("Player").child("target_speed").attribute("y").as_float();

	player->state = (PlayerState)data.child("Player").child("state").attribute("value").as_int();
	player->is_grounded = data.child("Player").child("is_grounded").attribute("value").as_bool();
	player->flipX = data.child("Player").child("flipX").attribute("value").as_bool();



 	return true;
}
