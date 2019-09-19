#include "j1SwapScene.h"
#include "p2Log.h"
#include "j1App.h"
#include <math.h>
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "SDL\include\SDL_render.h"
#include "SDL\include\SDL_timer.h"


j1MapChange::j1MapChange()
{
	name.assign("swap_scene");
}

j1MapChange::~j1MapChange()
{}

bool j1MapChange::Awake(pugi::xml_node&)
{
	uint scale =App->win->GetScale();
	screen = { 0, 0, (int)(App->win->width * scale), (int)(App->win->height * scale) };
	return true;
}

bool j1MapChange::Now()
{
	return true;
}

bool j1MapChange::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}


bool j1MapChange::Update(float dt)
{
	if (current_step == fade_step::none)
		return true;
	

	
	uint now = SDL_GetTicks() - start_time;
	

	float normalized = 1.0f < ((float)now / (float)total_time) ? 1.0f : ((float)now / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		App->entityFactory->player->spriteIncrease += 0.1;

		if (now >= total_time) //Point where the screen is totally black, and the new map is loaded.
		{
			App->audio->SetVolume(MIX_MAX_VOLUME);
			App->scene->maptoReset = nextMap;
			std::list<std::string>::const_iterator item = App->scene->map_names.begin();
			int i = 0;
			for (; item != App->scene->map_names.end(); ++item)
			{
				if (i == nextMap)
				{
					App->map->SwitchMaps((*item).data());
					App->entityFactory->player->lockInput = false;
				}

				++i;
			}
			total_time += total_time;
			start_time = SDL_GetTicks();
			fading = false;
			current_step = fade_step::fade_from_black;
			App->entityFactory->player->ResetPlayer();

		}
	}break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;
		App->entityFactory->player->spriteIncrease = 1.5f;

		if (now >= total_time)
		{
			
			current_step = fade_step::none;
			App->collision->Triggercolliding = false;
			
		}

	}break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool j1MapChange::ChangeMap(int newMap, float time)
{
	bool ret = false;

	nextMap = newMap;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		fading = true;
		ret = true;

	}

	return ret;
}

bool j1MapChange::IsChanging() const
{
	return current_step != fade_step::none;
}

bool j1MapChange::Save(pugi::xml_node& node) const
{
	pugi::xml_node scene_node;
	scene_node = node.append_child("scene");

	std::list<std::string>::iterator item = App->scene->map_names.begin();

	int i = 0;


	while (item != App->scene->map_names.end())
	{
		if (App->scene->actualMap == (*item).data()) 
		{
			scene_node.append_attribute("name") = (*item).data();

			if (App->scene->actualMap == "Level1.tmx") 
				App->scene->maptoReset = 0;

			else if (App->scene->actualMap == "Level2.tmx")
				App->scene->maptoReset = 1;

			return true;
		}
		++i;
		++item;
	}



	return true;
}



bool j1MapChange::Load(pugi::xml_node& node)
{
	std::string scene_name = node.child("scene").attribute("name").as_string();

	std::list<std::string>::iterator item = App->scene->map_names.begin();

	while (item != App->scene->map_names.end())
	{
		if (scene_name == (*item).data())
		{
			if (scene_name == "Level1.tmx")
			{
				App->scene->maptoReset = 0;
				App->audio->PlayMusic("audio/music/Parabola.ogg", -1);
			}

			else if (scene_name == "Level2.tmx")
			{
				App->scene->maptoReset = 1;
				App->audio->PlayMusic("audio/music/BillySacrifice.ogg", -1);
			}


			App->map->SwitchMaps((*item).data());
			return true;
		}

		++item;
	}
	
	return true;
}
