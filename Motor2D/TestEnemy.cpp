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
	return true;
}

bool TestEnemy::Update(float dt)
{

	currentAnimation = e_test_IDLE.GetCurrentFrame();

	//velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	//velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;

	//MovX();//need to review this (this function only works for player, need to set one for enemies
	//MovY();

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

	if (jump && total_jumps < MAX_JUMPS)
	{
		Jump();
	}
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

	iPoint new_destination = App->map->WorldToMap(App->entityFactory->player->pivot.x, App->entityFactory->player->pivot.y);

	if (new_destination != destination || current_path.Count() == 0)
	{
		iPoint origin = App->map->WorldToMap(pivot.x, pivot.y);
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
