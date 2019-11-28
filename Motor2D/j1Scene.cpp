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
#include "j1PathFinding.h"
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
	actualMap = map_names.begin()->data();
	App->audio->PlayMusic("audio/music/Parabola.ogg", -1);

	//pathfinding
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
	
	stepSFX = App->audio->LoadFx("audio/fx/footstep.wav");
	landSFX = App->audio->LoadFx("audio/fx/landing.wav");
	jumpSFX = App->audio->LoadFx("audio/fx/jump.wav");
	endLvlSFX = App->audio->LoadFx("audio/fx/EndLevel.wav");
	dashSFX = App->audio->LoadFx("audio/fx/dash.wav");
	bounceSFX = App->audio->LoadFx("audio/fx/bounce.wav");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{



	


	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		DoViewportResize();
		LOG("%i %i", App->render->camera.x, App->render->camera.y);
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
	{
		App->render->angles += 40;
	}

	//<------WIP------>
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
	{
		App->render->ResetViewPort();
	}
	if (viewportResize == true)
	{
		/*resizeTimer.Start();
		viewportResize = false;*/
	}

	PartyMaker();
	App->render->SetBackgroundColor({ red,green,blue });

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
	//	//App->render->ResetViewPort();
	//	App->render->camera.w = App->win->screen_surface->w;
	//	App->render->camera.h = App->win->screen_surface->h;
	//	/*App->render->camera.x = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y).x;
	//	App->render->camera.y = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y).y;*/

	//}
	//<------WIP------>
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{




	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	App->map->ResetBFS();

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//	App->map->PropagateBFS();


	



	
	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	 


	int x, y;

	App->input->GetMousePosition(x, y);
	iPoint alfa = { x,y };
	alfa = App->map->WorldToMap(alfa.x, alfa.y);

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		origin = { 10,10 };

		destination = { 16,10 };

		initpath = true;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && counter == 0)
	{
		origin = alfa;
		counter++;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && counter >= 1)
	{
		destination = alfa;
		counter = 0;
		initpath = true;
	}


	if (initpath)
	{

		App->pathfinding->CreatePath(origin, destination, 0);
		const p2DynArray<iPoint>* tmpArray = App->pathfinding->GetLastPath();
		current_path.Clear();
		for (int i = 0; i < tmpArray->Count(); ++i)
		{
			iPoint p = App->map->MapToWorld(tmpArray->At(i)->x, tmpArray->At(i)->y);
			p.x += App->map->data.tile_width / 2;
			p.y += App->map->data.tile_height / 2;
			current_path.PushBack(p);

		}

		initpath = false;
	}

	if (current_path.Count() > 0)
	{
		for (int i = 0; i < current_path.Count(); i++)
		{
			iPoint p = { current_path.At(i)->x, current_path.At(i)->y };
			p.x -= App->map->data.tile_width / 2;
			p.y -= App->map->data.tile_height / 2;

			SDL_Rect quad = { p.x, p.y, App->map->data.tile_width , App->map->data.tile_height };
			App->render->DrawQuad(quad, 255, 255, 0, 75, true);
		}
	}
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT)
	{
		App->entityFactory->player->spriteIncrease += 0.1;
	}


	if (App->entityFactory->player != nullptr)
	{
		CameraFollowingUpdate();

	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Loadlvl(0); //reset to first level (player, cam..)


	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		Loadlvl(1); // reset current level (player, cam..)

	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		ReLoadLevel();// reset current level (player, cam..)

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		//App->entityFactory->DeleteAllEnemies();
		App->LoadGame("save_game.xml");
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		App->capFrames ? App->capFrames = false, 
			App->entityFactory->player->norm_moves = App->GetInvers_AVG_FPS() : App->capFrames = true;

	}
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
		ret = true;

	else 
		currentMap = 0;
	
	return true;
}

bool j1Scene::Loadlvl(int lvl)
{

	
	std::list<std::string>::iterator item = App->scene->map_names.begin();
	int i = 0;
		for (; item != App->scene->map_names.end(); ++item)
		{
			if (i == lvl)
			{
				if (i == 1)
				{
					maptoReset = 1;
					App->audio->PlayMusic("audio/music/BillySacrifice.ogg", -1);

				}
				else
				{
					App->audio->PlayMusic("audio/music/Parabola.ogg", -1);
					maptoReset = 0;
				}

				App->swapScene->ChangeMap(maptoReset, fadeTime);
				return true;
			}

			++i;
		}
	return true;
}

void j1Scene::TriggerColl()
{

	
	if (maptoReset == 0)
		Loadlvl(1);
	

	else if (maptoReset == 1)
		Loadlvl(0);
	
	

}

void j1Scene::ReLoadLevel()
{
	
	
 	Loadlvl(maptoReset); 
	
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
		 blue += 150;

	  if (blue >= 255 && incB == true)
		 incB = false;

	  if (blue > 0 && incB == false)
		 blue -= 50;

	  else if (blue <= 0 == incB == false)
		 incB = true;

}

int j1Scene::LerpLabel(int v0, int v1, float t)
{
	return (v0 * (1-t) + v1 * t);
}

void j1Scene::CameraFollowingUpdate()
{
	
	iPoint playerPos = App->entityFactory->player->GetPosition();
	iPoint MapPOOs = App->map->MapToWorld(App->map->data.width, App->map->data.height);



	if (App->render->camera.x - App->render->camera.w - 50 < -MapPOOs.x) //going to right
	{
		if (playerPos.x > MapPOOs.x - SIDE_OFFSET)
		{
			stopX = true;
		}
		else if (playerPos.x < MapPOOs.x - SIDE_OFFSET)
		{
			stopX = false;
		}
	}

	else if (App->render->camera.x > -13) //going to left 
	{

		if (playerPos.x > SIDE_OFFSET)
		{
			stopX = false;
		}
		else if (playerPos.x <= SIDE_OFFSET)
		{
			stopX = true;
		}



	}
	else stopX = false;

	if (App->render->camera.y - App->render->camera.h - 50 < -MapPOOs.y) //going down
	{
		LOG("in bottom border");
		if (playerPos.y > BOTTOM_OFFSET)	stopY = true;
		
		else if (playerPos.y < BOTTOM_OFFSET) stopY = false;
	}

	if (App->render->camera.y >= 0) //going up
	{
		
		if (playerPos.y <= TOP_OFFSET)
		{
			App->render->camera.y = 0;
			stopY = true;
		}
		else if (playerPos.y >= TOP_OFFSET)
		{
			App->render->camera.y = 0;
 			stopY = false;
		}
	}

	if (!stopX)
		App->render->camera.x = LerpLabel(App->render->camera.x, -playerPos.x + App->render->camera.w * 0.5, TRAVEL_SPEED ); //testing numbers

	if (!stopY)
		App->render->camera.y = LerpLabel(App->render->camera.y, -playerPos.y + App->render->camera.h * 0.5, TRAVEL_SPEED );

}

