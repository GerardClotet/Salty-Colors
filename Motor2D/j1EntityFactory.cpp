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

	sprite_route = PATH(config.child("folder").child_value(), config.child("sprite").child_value());
	int animationID;
	for (auto node : config.child("player").child("animations").children("frame"))
	{
		animationID = node.attribute("id").as_int();

		if (animationID == 1)
		{
			player_IDLE.PushBack({node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int()});
		}
		else if(animationID ==2)
			player_RUN.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

	}
	player_IDLE.loop = true;
	player_IDLE.speed = 10.0f;
	player_RUN.loop = true;
	player_RUN.speed = 10.0f;


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


	//Load entities textures

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
				if ((*item)->type == ENTITY_TYPE::ENT_PLAYER)
				{
					//player = nullptr;
				}

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

bool j1EntityFactory::GetPlayerState()
{
	return playerActive;
}
