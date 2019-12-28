#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Input.h"

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



	if (App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN)
		draw_path = !draw_path;

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
		(*item)->Draw();
	}

	//std::vector<j1Entity*>::iterator drawItem = draw_entities.begin();
	//for (; drawItem != draw_entities.end(); ++drawItem)
	//{
	//	(*drawItem)->Draw();

	//	//here can draw debug for entitys
	//}

//	draw_entities.clear();

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









bool j1EntityFactory::GetPlayerState()const
{
	return playerActive;
}

void j1EntityFactory::CreateEntity(iPoint pos, E_TYPE type)
{

	switch (type)
	{
	case E_TYPE::NONE:
		break;
	case E_TYPE::PLAYER:

		if (player == nullptr)
		{
			player = new j1Player(pos);
			if (player)
				entities.push_back(player);
		}
		else
		{
			LOG("Player Already Create, changing his position");
			player->ResetPlayer();
			player->SetPos(pos);
		
		}
		break;
	case E_TYPE::WALK_E:
	{
		j1Enemy* en = new TestEnemy(pos);
		en->id = entities.size();
		entities.push_back(en);

		break; }
	case E_TYPE::FLY_E:
	{
		j1Enemy* en = new FlyingEnemy(pos);
		en->id = entities.size();
		entities.push_back(en);
		break;}
	default:
		break;
	}

}

bool j1EntityFactory::Save(pugi::xml_node& data) const
{

	std::list<j1Entity*>::const_iterator item = entities.begin();
	for (; item != entities.end(); ++item)
	{
		(*item)->Save(data);

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

void j1EntityFactory::DeleteAllEnemies()
{
	std::list<j1Entity*>::iterator it = entities.begin();

	while (it != entities.end())
	{
		if ((*it)->entity_type == E_TYPE::ENEMY)
		{
			(*it)->CleanUp();
			
			RELEASE(*it);
			delete (*it);
			*it = nullptr;
			entities.erase(it);
		}
	
		++it;
	}
}

void j1EntityFactory::DeleteEntity(j1Entity* entity)
{
	std::list<j1Entity*>::iterator it = entities.begin();

	while (it != entities.end())
	{
		if ((*it) == entity)
		{
			entities.remove(*it);
			return;
			
		}
		++it;
	}


}
