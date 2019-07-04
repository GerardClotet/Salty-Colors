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
	

	j1Timer now;

	now.Start();

	float normalized = 1.0f < ((float)now.Read() / (float)total_time) ? 1.0f : ((float)now.Read() / (float)total_time);

	switch (current_step)
	{
	case fade_step::fade_to_black:
	{
		if (now.Read() >= total_time) //Point where the screen is totally black, and the new map is loaded.
		{
			App->scene->currentMap = nextMap;
			std::list<std::string>::iterator item = App->scene->map_names.begin();
			int i = 0;
			for (; item != App->scene->map_names.end(); ++item)
			{
				if (i == nextMap)
				{
					App->map->SwitchMaps((*item).data());

				}

				++i;
			}
			//App->map->SwitchMaps(App->scene->map_names.assign[&nextMap]);
			//App->player->Restart();
			total_time += total_time;
			start_time = SDL_GetTicks();
			fading = false;
			current_step = fade_step::fade_from_black;
		}
	}break;

	case fade_step::fade_from_black:
	{
		normalized = 1.0f - normalized;

		if (now.Read() >= total_time)
		{
			current_step = fade_step::none;
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
		if (i == App->scene->currentMap)
		{
			scene_node.append_attribute("name") = (*item).data();
		}
		++i;
		++item;
	}


	return true;
}

/*App->scene->currentMap = nextMap;
std::list<std::string>::iterator item = App->scene->map_names.begin();
int i = 0;
for (; item != App->scene->map_names.end(); ++item)
{
	if (i == nextMap)
	{
		App->map->SwitchMaps((*item).data());

	}

	+i;
}*/

bool j1MapChange::Load(pugi::xml_node& node)
{
	std::string scene_name = node.child("scene").attribute("name").as_string();

	std::list<std::string>::iterator item = App->scene->map_names.begin();

	while (item != App->scene->map_names.end())
	{
		if (scene_name != (*item).data())
		{
			App->map->SwitchMaps((*item).data());
			
		}
		//else App->map->SwitchMaps((*item).data());

		++item;
	}
	
	return true;
}
