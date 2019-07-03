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

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("Level1.tmx");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
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

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
	{
		DoViewportResize();
	}

	if (viewportResize==true)
	{
		resizeTimer.Start();
		viewportResize = false;
	}

	if (resizeTimer.Read() < 10000 && resizeTimer.Read() > 10)
	{
		App->render->SetViewPort(RezieView(App->render->camera,false));
		App->render->camera = RezieView(App->render->camera,true);

	}
	else App->render->ResetViewPort();
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
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

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

bool j1Scene::DoViewportResize()
{
	return viewportResize = true;
}

SDL_Rect j1Scene::RezieView(SDL_Rect vp,bool cam)
{
	if (!cam)
	{
		vp.y += 1;
		vp.x += 1;
	}
	vp.h -= 1;
	vp.w -= 1;
	return vp;
}
