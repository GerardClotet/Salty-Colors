#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1SwapScene.h"
#include "j1EntityFactory.h"
#include "j1Player.h"
j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{

	LOG("Loading Scene");
	bool ret = true;

	fadeTime = config.child("fade_time").attribute("value").as_float();

	std::string data;
	for (auto node : config.children("map_name"))
	{
		data = node.attribute("name").as_string();
		map_names.push_back(data);
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(map_names.begin()->data());
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	
	//App->render->camera.x = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y).x;
	//App->render->camera.y = App->map->WorldToMap(App->entityFactory->player->pos.x, App->entityFactory->player->pos.y).y;

//	App->render->camera.x = App->entityFactory->player->pos.x;


	//if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	//{
	//	DoViewportResize();
	//	LOG("%i %i", App->render->camera.x, App->render->camera.y);
	//}
	//if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
	//{
	//	App->render->ResetViewPort();
	//}
	//if (viewportResize == true)
	//{
	//	resizeTimer.Start();
	//	viewportResize = false;
	//}

	//if (resizeTimer.Read() < 5000 && resizeTimer.Read() > 10)
	//{
	//	if (RezieView(App->render->camera, false).w != App->entityFactory->player->collider->rect.x + App->entityFactory->player->collider->rect.w)
	//	{
	//		App->render->SetViewPort(RezieView(App->render->camera, false));

	//	}
	//	App->render->SetViewPort(RezieView(App->render->camera, false));
	//	App->render->camera = RezieView(App->render->camera, false);

	//	/*PartyMaker();
	//	App->render->SetBackgroundColor({ red,green,blue });*/
	//}
	//else
	//{
	//	App->render->ResetViewPort();
	//	App->render->camera.w = App->win->screen_surface->w;
	//	App->render->camera.h = App->win->screen_surface->h;
	//	/*App->render->camera.x = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y).x;
	//	App->render->camera.y = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y).y;*/

	//}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Loadlvl(0); //here to reset to first level (player, cam..)
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		int temporal = 1; //here to reset current level (player, cam..)

	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		Loadlvl(1);

	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 5;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 5;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 5;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 5;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		App->map->ResetBFS();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->map->PropagateBFS();


	App->map->Draw();


	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
	//				App->map->data.width, App->map->data.height,
	//				App->map->data.tile_width, App->map->data.tile_height,
	//				App->map->data.tilesets.size(),
	//				map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		LOG("current map %i", currentMap);
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		SwapMap(); //not working always stays in the same map, oriented to load from saves or finish level

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");


	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::SwapMap()
{
	bool ret = true;
	if (currentMap < map_names.size() - 1)
		ret = App->swapScene->ChangeMap(++currentMap, fadeTime);

	else
	{
		currentMap = 0;
		App->swapScene->ChangeMap(currentMap, fadeTime);
	}
	return false;
}

bool j1Scene::Loadlvl(int lvl)
{
	//App->map->SwitchMaps()

	
	std::list<std::string>::iterator item = App->scene->map_names.begin();
	int i = 0;
		for (; item != App->scene->map_names.end(); ++item)
		{
			if (i == lvl)
			{
				if (i == 1)
					maptoSwap = 0;

				else maptoSwap = 1;

				App->map->SwitchMaps((*item).data());

				return true;
			}

			++i;
		}
	return true;
}

bool j1Scene::OnCollision(Collider* c1, Collider* c2)
{

	if (c2->type == COLLIDER_PLAYER) //&& player state != WIN (to trigger final animation)
	{
		if (maptoSwap == 0) //should try with currentMap var!!! need to be tested
			Loadlvl(1);

		else if (maptoSwap == 1)
			Loadlvl(0);
	}


	return true;
}

bool j1Scene::DoViewportResize()
{
	return viewportResize = true;
}

SDL_Rect j1Scene::RezieView(SDL_Rect vp,bool cam)
{
	if (!cam)
	{
	/*	vp.y -= 0.5;
		vp.x += 0.5;*/
	}
	vp.h -= 1;
	vp.w -= 5;
	return vp;
}

void j1Scene::PartyMaker()
{

	if (red < 255 && incR==true)
		red += 1;
	
	 if (red == 255 && incR == true)
		incR = false;
	
	if (red > 0 && incR == false)
		red -= 4;

	else if (red == 0 && incR == false)
		incR = true;



	if (green < 255 && incG == true)
		green += 6;

	 if (green >= 255 && incG == true)
		incG = false;

	if (green > 0 && incG == false)
		green -= 20;

	 if (green <= 0==incG == false)
		incG = true;


	 if (blue < 255 && incB == true)
		 blue += 5;

	  if (blue >= 255 && incB == true)
		 incB = false;

	  if (blue > 0 && incB == false)
		 blue -= 50;

	  else if (blue <= 0 == incB == false)
		 incB = true;

}
