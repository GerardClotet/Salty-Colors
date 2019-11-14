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




bool j1Enemy::CleanUp()
{
	
	return true;
}