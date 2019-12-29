#include "j1Render.h"
#include "FlyingEnemy.h"
#include "j1Map.h"
#include "j1EntityFactory.h"
#include "j1Pathfinding.h"
#include "p2Log.h"

#include "j1Textures.h"
#include "j1Scene.h"

FlyingEnemy::FlyingEnemy(iPoint pos) : j1Enemy(pos) {

	AwakeAttributes(App->config);
	entityTex = App->entityFactory->fly_enemy_tex;

	//LOG("%s", sprite_route.data());

	position = pos;
	animation_Coll = { 0,0, 19, 30 };

	collider = App->collision->AddCollider({0,0,32,32}, COLLIDER_FLYING, App->entityFactory, true);
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	
	state = PlayerState::MOVING;
	LOG("Created FlyingEnemy");

}

FlyingEnemy::~FlyingEnemy()
{
	collider = nullptr;
}

bool FlyingEnemy::PreUpdate()
{
	//Check start chase
	if (position.DistanceManhattan(App->entityFactory->player->position) < chase_distance && App->entityFactory->player->state != GOD)
		chase = true;
	else
	{
		current_path.Clear();
		chase = false;
		ResetPathfindingVariables();
	}
	PathfindingPreupdate();

	switch (state)
	{
	case MOVING: MovingUpdate();
		break;
	case DEAD:
		break;
	default:
		break;
	}

	return true;
}

bool FlyingEnemy::Update(float dt)
{
	
	PathfindingUpdate();

	MovY();
	MovX();


	velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;

	currentAnimation = IDLE_Flying.GetCurrentFrame();

	return true;
}

bool FlyingEnemy::CleanUp()
{
	if (collider)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

void FlyingEnemy::SetPos(iPoint pos)
{
}

bool FlyingEnemy::Load(pugi::xml_node &data)
{
	for (auto node : data.child("enemy"))
	{
		if (node.attribute("id").as_int() == id)
		{
			position.x = data.child("enemy").child("FlyingEnemy").attribute("x").as_int();
			position.y = data.child("enemy").child("FlyingEnemy").attribute("y").as_int();

			flipX = data.child("enemy").child("FlyingEnemy").child("flipX").attribute("value").as_bool();

			return true;
		}
	}
	


	return true;
}

bool FlyingEnemy::Save(pugi::xml_node &data) const
{
	pugi::xml_node e_data;
	
	e_data = data.append_child("enemy").append_child("FlyingEnemy");
	e_data.append_attribute("id") = id;
	e_data.append_attribute("x") = position.x;
	e_data.append_attribute("y") = position.y;

	e_data.append_child("flipX").append_attribute("value") = flipX;

	return true;
}

bool FlyingEnemy::AwakeAttributes(pugi::xml_node config)
{
	sprite_route = PATH(config.child("entityFactory").child("FlyingEnemy").child("folder").child_value(), config.child("entityFactory").child("FlyingEnemy").child("sprite").child_value());
	
	movement_speed = config.child("entityFactory").child("FlyingEnemy").child("movement_speed").attribute("value").as_float();

	acceleration = config.child("entityFactory").child("FlyingEnemy").child("acceleration").attribute("value").as_float();

	int animationID;

	for (auto node : config.child("entityFactory").child("FlyingEnemy").child("animations").children("frame"))
	{
		LOG("IMMA INNNN BOI");
		animationID = node.attribute("id").as_int();
		if (animationID == 1)
			IDLE_Flying.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	IDLE_Flying.loop = true;
	IDLE_Flying.speed = 10.0f;


	return true;
}


void FlyingEnemy::MovingUpdate()
{
	if (moving_left == moving_right)
	{
		target_speed.x = 0.0F;
	}
	else if (moving_right)
	{
		target_speed.x = movement_speed;
		flipX = false;
	}
	else if (moving_left)
	{
		target_speed.x = -movement_speed;
		flipX = true;
	}

	if (jump == moving_down)
	{
		target_speed.y = 0.0F;
	}
	else if (jump)
	{
		target_speed.y = -movement_speed;
	}
	else if (moving_down)
	{
		target_speed.y = movement_speed;
	}
}

bool FlyingEnemy::GetPath()
{
	iPoint new_destination = App->map->WorldToMap(App->entityFactory->player->position.x, App->entityFactory->player->position.y);

	if (new_destination != destination || current_path.Count() == 0)
	{
		iPoint origin = App->map->WorldToMap(position.x, position.y);
		destination = new_destination;

		App->pathfinding->CreatePath(origin, destination, jump_height);

		const p2DynArray<iPoint>* tmp_array = App->pathfinding->GetLastPath();
		current_path.Clear();
		for (int i = 0; i < tmp_array->Count(); i++)
		{
			iPoint p = App->map->MapToWorld(tmp_array->At(i)->x, tmp_array->At(i)->y);
			p.x += App->map->data.tile_width / 2;
			p.y += App->map->data.tile_height / 2 + App->entityFactory->player->spriteIncrease;
			current_path.PushBack(p);
		}
		current_destination = current_path.Count() > 1 ? 1 : 0;
		previous_destination = 0;
		next_destination = current_path.Count() > 2 ? 2 : -1;

		ResetPathfindingVariables();
	}
	return true;
}

void FlyingEnemy::ResetPathfindingVariables()
{
	moving_right = false;
	moving_left = false;
	jump = false;
	moving_down = false;
}

void FlyingEnemy::DrawPath()
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

void FlyingEnemy::PathfindingUpdate()
{
	if (chase)
		GetPath();
	if (App->entityFactory->draw_path)
		DrawPath();
}

void FlyingEnemy::PathfindingPreupdate()
{
	if (current_path.Count() > 0)
	{
		ResetPathfindingVariables();

		PathfindX();
		PathfindY();

		if (reached_X && reached_Y)
		{
			previous_destination = current_destination;
			current_destination++;
			next_destination = current_destination + 1;

			if (next_destination >= current_path.Count())
				next_destination = -1;

			if (current_destination >= current_path.Count())
				current_path.Clear();
		}
	}
}

void FlyingEnemy::PathfindX()
{
	reached_X = (current_path.At(previous_destination)->x <= current_path.At(current_destination)->x && current_path.At(current_destination)->x <= pivot.x)
		|| (current_path.At(previous_destination)->x >= current_path.At(current_destination)->x && current_path.At(current_destination)->x >= pivot.x);


	if (!reached_X)
	{

		if (position.x < current_path.At(current_destination)->x)
			moving_right = true;

		else if (position.x > current_path.At(current_destination)->x)
			moving_left = true;

	}
	else
	{
		if (next_destination > 0)
		{
			iPoint point = App->map->WorldToMap(current_path.At(next_destination)->x, current_path.At(next_destination)->y);
			if (!App->pathfinding->IsGround({ point.x, point.y + 1 }))
			{
				moving_right = false;
				moving_left = false;
			}
			else
			{

				if (!CheckifHasReachedDesPos(current_path.At(next_destination)->x, position.x))
				{

					//int guarropos2 = 
					if (position.x < current_path.At(next_destination)->x)
					{
						previous_pos = position.x; //previ
						moving_right = true;
					}
					else if (position.x > current_path.At(next_destination)->x)
					{
						previous_pos = position.x; //previ
						moving_left = true;
					}
				}
			}
		}
	}
}

void FlyingEnemy::PathfindY()
{
	reached_Y = (current_path.At(previous_destination)->y <= current_path.At(current_destination)->y && position.y >= current_path.At(current_destination)->y)
		|| (current_path.At(previous_destination)->y >= current_path.At(current_destination)->y && position.y <= current_path.At(current_destination)->y);

	if (abs(position.y - current_path.At(current_destination)->y) > POSITION_ERROR_Y)
		reached_Y = false;

	if (!reached_Y)
	{
		if (position.y > current_path.At(current_destination)->y)
			jump = true;
		else if (position.y < current_path.At(current_destination)->y)
			moving_down = true;
	}
}

