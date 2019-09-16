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

}

bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&) const
{
	return true;
}




iPoint j1Entity::GetPosition()
{
	return position;
}

void j1Entity::MovX()
{
	if (state != GOD)
	{
		if (velocity.x > 0)
		{
			velocity.x = MIN(velocity.x, App->collision->DistanceToRightCollider(collider));

		}
		else if (velocity.x < 0)
		{
			velocity.x = MAX(velocity.x, App->collision->DistanceToLeftCollider(collider));
		}
	}


	

	position.x += velocity.x;
	collider->rect.x = position.x;
}

void j1Entity::MovY()
{
	if (state != GOD)
	{
		if (velocity.y < 0)
		{
			velocity.y = MAX(velocity.y, App->collision->DistanceToTopCollider(collider));
			if (velocity.y == 0) target_speed.y = 0.0F;
		}
		else
		{
			float distance = App->collision->DistanceToBottomCollider(collider, ignore_platforms);
			velocity.y = MIN(velocity.y, distance);
			is_grounded = (distance == 0) ? true : false;

		}

	}



	position.y += velocity.y;
	collider->rect.y = position.y; 

}


