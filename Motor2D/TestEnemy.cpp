#include "j1Render.h"
#include "TestEnemy.h"
#include "j1EntityFactory.h"
#include "j1PathFinding.h"
#include "j1Map.h"
#include "p2Log.h"


TestEnemy::TestEnemy(iPoint pos) : j1Enemy(pos)
{
	AwakeAttributes(App->config);
	entityTex = App->tex->Load(sprite_route.data());
	
	LOG("%s", sprite_route.data());
	position = pos;
	animation_Coll = { 0,0,52,41 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_ENEMY, App->entityFactory, true);
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	LOG("Created TestEnemy");
}

TestEnemy::~TestEnemy()
{
}


bool TestEnemy::PreUpdate()
{
	if (position.DistanceManhattan(App->entityFactory->player->position) < chase_distance)
		chase = true;
	else
		chase = false;

	PathfindingPreupdate();

	switch (state) {
	case IDLE: IdleUpdate();
		break;
	case MOVING: MovingUpdate();
		break;
	case JUMPING: JumpingUpdate();
		break;
	case DEAD:
		break;
	default:
		break;
	}

	
	return true;
}

bool TestEnemy::Update(float dt)
{

	PathfindingUpdate();
	currentAnimation = e_test_IDLE.GetCurrentFrame();

	

	if (state == JUMPING)
	{
		target_speed.y += gravity * dt;
		if (target_speed.y > fall_speed) target_speed.y = fall_speed; //limit falling speed
	}

	
	MovX();
	MovY();

	velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;


	//Draw();
	return true;
}

//void TestEnemy::Draw()
//{
//	if(entityTex != nullptr)
//		App->render->Blit(entityTex, position.x, position.y, &currentAnimation, 1.0f, flipX, false);
//	//For some reason the texture isnt rendering
//}

void TestEnemy::SetPos(iPoint pos)
{
	position = pos;
	if (collider)
		collider->SetPos(position.x, position.y);
}

bool TestEnemy::CleanUp()
{
	if (collider)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

bool TestEnemy::Load(pugi::xml_node&data)
{
	position.x = data.child("enemy").child("TestEnemy").attribute("x").as_int();
	position.y = data.child("enemy").child("TestEnemy").attribute("y").as_int();

	flipX = data.child("enemy").child("TestEnemy").child("flipX").attribute("value").as_bool();

	return true;
}

bool TestEnemy::Save(pugi::xml_node&data) const
{
	pugi::xml_node e_data;

	e_data = data.append_child("enemy").append_child("TestEnemy");

	e_data.append_attribute("x") = position.x;
	e_data.append_attribute("y") = position.y;

	e_data.append_child("flipX").append_attribute("value") = flipX;

	return true;
}

bool TestEnemy::AwakeAttributes(pugi::xml_node config)
{
	//movement_speed = config.
	sprite_route = PATH(config.child("entityFactory").child("enemy").child("folder").child_value(), config.child("entityFactory").child("enemy").child("sprite").child_value());

	
		movement_speed = config.child("entityFactory").child("enemy").child("movement_speed").attribute("value").as_float();

		jump_speed = config.child("entityFactory").child("enemy").child("jump_speed").attribute("value").as_float();

		gravity = config.child("entityFactory").child("enemy").child("gravity").attribute("value").as_float();

		acceleration = config.child("entityFactory").child("enemy").child("acceleration").attribute("value").as_float();

		fall_speed = config.child("entityFactory").child("enemy").child("fall_speed").attribute("value").as_float();

		bounce_speed = config.child("entityFactory").child("enemy").child("bounce_speed").attribute("value").as_float();


	int animationID;
	for (auto node : config.child("entityFactory").child("enemy").child("animations").children("frame"))
	{
		animationID = node.attribute("id").as_int();
		if(animationID ==1)
			e_test_IDLE.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });


	}

	e_test_IDLE.loop = true;
	e_test_IDLE.speed = 10.0f;

	return true;
}
void TestEnemy::IdleUpdate()
{
	target_speed.x = 0.0F;
	if (moving_left != moving_right)
		state = MOVING;
	if (jump) Jump();

	if (!is_grounded) state = JUMPING;
}

void TestEnemy::MovingUpdate()
{

	if (moving_left == moving_right)
	{
		state = IDLE;
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

	if (jump)
	{
		Jump();
	}

	if (!is_grounded)
		state = JUMPING;

}

void TestEnemy::JumpingUpdate()
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

	if (is_grounded)
	{
 		if (moving_left == moving_right) state = IDLE;
		else state = MOVING;

		target_speed.y = 0.0F;
		velocity.y = 0.0F;
		total_jumps = 0;
	}

	//if (jump && total_jumps < MAX_JUMPS)
	//{
	//	Jump();
	//}
}


void TestEnemy::Jump()
{
	target_speed.y = -jump_speed;
	is_grounded = false;
	state = JUMPING;
	total_jumps++;
}


bool TestEnemy::GetPath()
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

void TestEnemy::ResetPathfindingVariables()
{
	moving_right = false;
	moving_left = false;
	jump = false;
}

void TestEnemy::PathfindingUpdate()
{
	if (chase && state != JUMPING)
		GetPath();
	if (App->entityFactory->draw_path)
		DrawPath();
}

void TestEnemy::PathfindingPreupdate()
{
	if (current_path.Count() > 0)
	{
		ResetPathfindingVariables();

		PathfindX();
		PathfindY();

		if (reached_X /*&& reached_Y*/)
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

void TestEnemy::PathfindX()
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
						guarropos = position.x; //previ
							moving_right = true;
					}
					else if (position.x > current_path.At(next_destination)->x)
					{
						guarropos = position.x; //previ
						moving_left = true;
					}
				}
			}
		}
	}
}






void TestEnemy::PathfindY()
{
	reached_Y = (current_path.At(previous_destination)->y <= current_path.At(current_destination)->y && position.y >= current_path.At(current_destination)->y)
		|| (current_path.At(previous_destination)->y >= current_path.At(current_destination)->y && position.y <= current_path.At(current_destination)->y);

	if (!reached_Y)
	{
		if (position.y > current_path.At(current_destination)->y)
			jump = true;
	}
}



void TestEnemy::DrawPath()
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