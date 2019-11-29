#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"


j1Entity::j1Entity(iPoint position)
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
		float distance;
		if (velocity.x > 0)
		{
			distance = App->collision->DistanceToRightCollider(collider);

			if (distance == 0)
			{
				App->entityFactory->player->ready_toBounce_right = true;
				App->entityFactory->player->flipX = true;
				App->entityFactory->player->in_contact = true;
				LOG("aah");
			}
			else App->entityFactory->player->ready_toBounce_right = false;
	

			velocity.x = MIN(velocity.x, distance);

		}
		else if (velocity.x < 0)
		{
			distance = App->collision->DistanceToLeftCollider(collider);

			if (distance == 0)
			{
				App->entityFactory->player->ready_toBounce_left = true;
				App->entityFactory->player->flipX = false;
				App->entityFactory->player->in_contact = true;

			}
			else App->entityFactory->player->ready_toBounce_left = false;
			velocity.x = MAX(velocity.x,distance);
		}
	}


	/**/

	position.x += velocity.x;
	collider->rect.x = position.x;
}

void j1Entity::MovY()
{
	if (state != GOD)
	{
		float distance;
		if (velocity.y < 0)
		{
			distance = App->collision->DistanceToTopCollider(collider);
			
			velocity.y = MAX(velocity.y, distance);
			if (velocity.y == 0) target_speed.y = 0.0F;
		}
		else
		{
			 distance = App->collision->DistanceToBottomCollider(collider, ignore_platforms);
			velocity.y = MIN(velocity.y, distance);
			is_grounded = (distance == 0) ? true : false;

		}

	}



	position.y += velocity.y;
	collider->rect.y = position.y; 

}

void j1Entity::SetPos(iPoint pos)
{
	
		position = pos;
		if (collider)
			collider->SetPos(position.x, position.y);

	
}




