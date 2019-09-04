#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"


j1Entity::j1Entity(ENTITY_TYPE type, iPoint position)
{
	LOG("entity constructed");
}

j1Entity::~j1Entity()
{

}


bool j1Entity::Start()
{
	return true;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::Update(float dt)
{
	Draw();
	return true;
}
bool j1Entity::PostUpdate()
{

	return true;
}

bool j1Entity::CleanUp()
{

	return true;
}

void j1Entity::Draw()
{
	//if (entityTex != nullptr)
	//	App->render->Blit(entityTex, position.x, position.y, &currentAnimation.GetCurrentFrame()); //or animation

	/*else if(currentAnimation != nullptr)
		App->render->Blit()*/

	LOG("entity drawn");
}

bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&) const
{
	return true;
}


void j1Entity::MovX()
{
	if (velocity.x > 0)
	{
		velocity.x = MIN(velocity.x, App->collision->DistanceToRightCollider(collider));
	
	}
	else if (velocity.x < 0)
	{
		velocity.x = MAX(velocity.x, App->collision->DistanceToLeftCollider(collider));
	}

	if (fabs(velocity.x) < threshold)
	{

		velocity.x = 0.0F;
	}

	position.x += velocity.x;
	collider->rect.x = position.x;
	pivot.x = position.x + (collider->rect.w / 2);
}

void j1Entity::MovY()
{
	if (velocity.y < 0)
	{
		LOG("velocity Y %f", velocity.y);
 		velocity.y = MAX(velocity.y, App->collision->DistanceToTopCollider(collider));
		if (velocity.y == 0) target_speed.y = 0.0F;
  		LOG("velocityY %f targetY %f", velocity.y, target_speed.y);
		//MAI pasa a ser 0
	}
	else
	{
		LOG("velocity Y %f", velocity.y);
		float distance = App->collision->DistanceToBottomCollider(collider, ignore_platforms);
	//	LOG("velocity Y %f", velocity.y);
		velocity.y = MIN(velocity.y, distance);
		is_grounded = (distance == 0) ? true : false;
		LOG("velocityY %f targetY %f", velocity.y, target_speed.y);

	}

	

	//if (fabs(velocity.y) < threshold) velocity.y = 0.0F;

	position.y += velocity.y;
	collider->rect.y = position.y; //+ coll_offSet;

}


