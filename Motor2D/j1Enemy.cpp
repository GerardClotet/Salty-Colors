#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Enemy.h"
#include "j1EntityFactory.h"

j1Enemy::j1Enemy(iPoint pos) : j1Entity(pos) //DRUM aquesta classe ha d'estar buida o lo minim, classe que forma els enemics
{

	entity_type = E_TYPE::ENEMY;
}



j1Enemy::~j1Enemy() {}

bool j1Enemy::Start()
{
	return true;
}

bool j1Enemy::PreUpdate()

{


	return true;
}


bool j1Enemy::Update(float dt)
{

	

	return true;

}

void j1Enemy::Draw()
{
	if(entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation, 1.0f, flipX, false);

}

void j1Enemy::IdleUpdate()
{
	target_speed.x = 0.0F;
	if (moving_left != moving_right)
		state = MOVING;
	if (jump) Jump();

	if (!is_grounded) state = JUMPING;
}

void j1Enemy::MovingUpdate()
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

void j1Enemy::JumpingUpdate()
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

void j1Enemy::Jump()
{
}




bool j1Enemy::CleanUp()
{
	target_speed.y = -jump_speed;
	is_grounded = false;
	state = JUMPING;
	total_jumps++;

	
	return true;
}