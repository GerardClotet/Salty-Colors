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


//It will be substitued with a general function to create Entities
j1Player* j1EntityFactory::CreatePlayer(iPoint pos)
{
	player = new j1Player(pos);

	if (player != nullptr)
	{
		entities.push_back(player);
		playerActive = true;
		return player;
	}
	else LOG("Failed to create player");
	return nullptr;
}


j1Enemy* j1EntityFactory::CreateEnemy(ENTITY_TYPE type, iPoint pos)


{
	p2SString id;

	switch (type)
	{
	case ENT_ENEMY:
		break;
	case ENT_FLYING:
		break;
	default:
		break;
	}
}



bool j1EntityFactory::GetPlayerState()
{
	return playerActive;
}

bool j1EntityFactory::Save(pugi::xml_node& file) const
{

	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Save(file.append_child(entity->data->id.GetString()));
	}
	return true;
	



	return true;
}

bool j1EntityFactory::LoadEnemies(pugi::xml_node &file)
{
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Load(file.child(entity->data->id.GetString()));
	}

	return true;
}

bool j1EntityFactory::Load(pugi::xml_node& data)
{
	
	std::list<j1Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->Load(data);

	}

 	return true;
}


