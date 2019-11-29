#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Enemy.h"
#include "j1Pathfinding.h"
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

void j1Enemy::MovX()
{
	if (state != DEAD)
	{
		if (velocity.x > 0) velocity.x = MIN(velocity.x, App->collision->DistanceToRightCollider(collider)); //movement of the player is min between distance to collider or his velocity
		else if (velocity.x < 0) velocity.x = MAX(velocity.x, App->collision->DistanceToLeftCollider(collider)); //movement of the player is max between distance to collider or his velocity
	}
	position.x += velocity.x;
	collider->rect.x = position.x;
	pivot.x = position.x + (collider->rect.w / 2);
}

void j1Enemy::MovY()
{
	if (state != DEAD)
	{
		if (velocity.y < 0)
		{
			velocity.y = MAX(velocity.y, App->collision->DistanceToTopCollider(collider)); //movement of the player is max between distance to collider or his velocity
			if (velocity.y == 0) target_speed.y = 0.0F;
		}
		else
		{
			float distance = App->collision->DistanceToBottomCollider(collider, ignore_platforms);
			velocity.y = MIN(velocity.y, distance); //movement of the player is min between distance to collider or his velocity
			is_grounded = (distance == 0) ? true : false;
		}
	}
	position.y += velocity.y;
	collider->rect.y = position.y + spriteIncrease;
	pivot.y = position.y + (collider->rect.h / 2);

}

bool j1Enemy::GetPath()
{
	return true;
}

void j1Enemy::ResetPathfindingVariables()
{
}

void j1Enemy::DrawPath()
{
}

void j1Enemy::PathfindingUpdate()
{
}

void j1Enemy::PathfindingPreupdate()
{
}

void j1Enemy::PathfindX()
{
}

void j1Enemy::PathfindY()
{
}



bool j1Enemy::CheckifHasReachedDesPos(int des, int current)
{
	if (previous_pos == -1)
		return false;
	if (previous_pos < des && current > des) // if previous pos was lower than dest_pos && current pos i bigger than dest
	{
		return true;
	}
	if (previous_pos > des && current < des) // if previous pos was higher than dest_pos && current pos i lower than dest
	{
		return true;
	}
	return false;
}






bool j1Enemy::CleanUp()
{
	
	
	return true;
}