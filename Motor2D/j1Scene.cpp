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
#include "j1Gui.h"

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

	/**///MainMenu///**/

		main_menu_panel = App->gui->CreateImage({ 2,2 }, { 957,0,873,960 });
		main_menu_panel->scale_Y = 1.17F;
		main_menu_button_play = App->gui->CreateButton({ 100, 75 }, main_menu_panel);
		main_menu_button_continue = App->gui->CreateButton({ 100, 175 }, main_menu_panel);
		main_menu_button_settings = App->gui->CreateButton({ 100, 275 }, main_menu_panel);
		main_menu_button_credits = App->gui->CreateButton({ 100, 375 }, main_menu_panel);
		main_menu_button_exit = App->gui->CreateButton({ 100, 475 }, main_menu_panel);

		main_menu_button_play_text = App->gui->CreateLabel({ 58,22 }, "fonts/open_sans/OpenSans-Bold.ttf", 28, "PLAY", { 255,255,255 }, main_menu_button_play);
		main_menu_button_continue_text = App->gui->CreateLabel({ 17,22 }, "fonts/open_sans/OpenSans-Bold.ttf", 28, "CONTINUE", { 255,255,255 }, main_menu_button_continue);
		main_menu_button_settings_text = App->gui->CreateLabel({ 25,22 }, "fonts/open_sans/OpenSans-Bold.ttf", 28, "SETTINGS", { 255,255,255 }, main_menu_button_settings);
		main_menu_button_credits_text = App->gui->CreateLabel({ 35,22 }, "fonts/open_sans/OpenSans-Bold.ttf", 28, "CREDITS", { 255,255,255 }, main_menu_button_credits);
		main_menu_button_exit_text = App->gui->CreateLabel({ 60,22 }, "fonts/open_sans/OpenSans-Bold.ttf", 28, "EXIT", { 255,255,255 }, main_menu_button_exit);
	
	///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

		/**///PauseMenu///**/

		pause_menu_panel = App->gui->CreateImage({ 50,50 }, { 147,854,645,736 });
		pause_menu_panel->scale_Y = 0.7F;
		pause_menu_panel->scale_X = 0.6F;
		pause_menu_button_resume = App->gui->CreateButton({ 90, 75 }, pause_menu_panel);
		pause_menu_button_main_menu = App->gui->CreateButton({ 90, 175 }, pause_menu_panel);
		pause_menu_button_resume_text = App->gui->CreateLabel({ 48,26 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "RESUME", { 255,255,255 }, pause_menu_button_resume);
		pause_menu_button_main_menu_text = App->gui->CreateLabel({ 60,14 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MAIN", { 255,255,255 }, pause_menu_button_main_menu);
		pause_menu_button_main_menu_text = App->gui->CreateLabel({ 56,36 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MENU", { 255,255,255 }, pause_menu_button_main_menu);



	///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

		/**///SettingsMenu///**/

		settings_menu_panel = App->gui->CreateImage({ 450,50 }, { 551,711,380,539 });
		settings_menu_panel->scale_Y = 0.6F;
		settings_menu_button_main_menu = App->gui->CreateButton({ 100, 180 }, settings_menu_panel);
		settings_menu_button_main_menu_text = App->gui->CreateLabel({ 60,14 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MAIN", { 255,255,255 }, settings_menu_button_main_menu);
		settings_menu_button_main_menu_text = App->gui->CreateLabel({ 56,36 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MENU", { 255,255,255 }, settings_menu_button_main_menu);

	///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

		/**///CreditsMenu///**/

		credits_menu_panel = App->gui->CreateImage({ 450,50 }, { 551,711,380,539 });
		credits_menu_panel->scale_Y = 0.9F;
		credits_menu_button_main_menu = App->gui->CreateButton({ 100, 320 }, credits_menu_panel);
		credits_menu_button_main_menu_text = App->gui->CreateLabel({ 60,14 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MAIN", { 255,255,255 }, credits_menu_button_main_menu);
		credits_menu_button_main_menu_text = App->gui->CreateLabel({ 56,36 }, "fonts/open_sans/OpenSans-Bold.ttf", 22, "MENU", { 255,255,255 }, credits_menu_button_main_menu);

	///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

	stepSFX = App->audio->LoadFx("audio/fx/footstep.wav");
	landSFX = App->audio->LoadFx("audio/fx/landing.wav");
	jumpSFX = App->audio->LoadFx("audio/fx/jump.wav");
	endLvlSFX = App->audio->LoadFx("audio/fx/EndLevel.wav");
	dashSFX = App->audio->LoadFx("audio/fx/dash.wav");
	bounceSFX = App->audio->LoadFx("audio/fx/bounce.wav");
	coinSFX = App->audio->LoadFx("audio/fx/coin.wav");

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



	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	
	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	 




	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT)
	{
		App->entityFactory->player->spriteIncrease += 0.1;
	}


	if (App->entityFactory->player != nullptr)
	{
		CameraFollowingUpdate();

	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		LOG("POLLA COMA UNA CASA");

		if (menu_Active == false)
		{
			menu_Active = true;
		}
		else menu_Active = false;
	
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Loadlvl(0); //reset to first level (player, cam..)


	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		Loadlvl(1); // reset current level (player, cam..)

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		Loadlvl(2);
	}

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

	App->entityFactory->CleanUp();
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
				else if(i == 0)
				{
					App->audio->PlayMusic("audio/music/Parabola.ogg", -1);
					maptoReset = 0;
				}
				else if (i == 2)
				{
			
					App->audio->PlayMusic("audio/music/MainMenuMusic.ogg", -1);
						maptoReset = 2;
						App->entityFactory->CleanUp();
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
	App->entityFactory->CleanUp();
	
 	Loadlvl(maptoReset); 
	
}

bool j1Scene::DoViewportResize()
{
	return viewportResize = true;
}

bool j1Scene::GUIEvent(j1UIElement* element, GUI_Event gui_event)
{
	switch (gui_event)
	{
	case MOUSE_OVER:
	{

	}
	break;
	case MOUSE_EXIT:
	{

	}
	break;
	}

	return true;
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

