#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Enemy.h"
#include "j1EntityFactory.h"

j1Enemy::j1Enemy(iPoint pos) : j1Entity(ENT_ENEMY, pos)
{
	entityTex = App->tex->Load(App->entityFactory->sprite_route.data());
	position = pos;

	animation_Coll = { 0,0,0,0 };

	collider->rect.x = position.x;
	collider->rect.y = position.y;
}



j1Enemy::~j1Enemy() {}

bool j1Enemy::Start()
{
	return true;
}

bool j1Enemy::PreUpdate()

{
	switch (state)
	{
	case NO_STATE:
		break;
	case IDLE: 
		break;
	case MOVING:
		break;
	case JUMPING: 
		break;
	case DEAD:
		break;
	default:
		break;
	}

	return true;
}


bool j1Enemy::Update(float dt)
{

	velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;

	MovX();
	MovY();

	Draw();
	if (state != DEAD)
		

	return true;

}




bool j1Enemy::CleanUp()
{
	if (collider)
	{

		collider->to_delete = true;
		collider = nullptr;

	}

	if (!is_grounded) state = JUMPING;

	return true;
}