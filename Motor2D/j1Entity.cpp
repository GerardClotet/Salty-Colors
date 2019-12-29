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
	collider = nullptr;

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
	if (state != GOD && collider != nullptr)
	{
		Collider* type;
		float distance;
		bool check = false;

		if (velocity.x >= 0)
		{
			distance = App->collision->DistanceToRightCollider(collider,type);

			if (distance == 0)
			{
				App->entityFactory->player->ready_toBounce_right = true;
				App->entityFactory->player->flipX = true;
				App->entityFactory->player->in_contact = true;
				if (type->type == COLLIDER_ENEMY && collider->type == COLLIDER_PLAYER)
				{
					dead = true;
					App->entityFactory->player->Die();
				}
			}
			else App->entityFactory->player->ready_toBounce_right = false;
	

			velocity.x = MIN(velocity.x, distance);
			check = true;

		}
		else if (velocity.x <= 0)
		{
			
			distance = App->collision->DistanceToLeftCollider(collider, type);

			if (distance == 0)
			{
				App->entityFactory->player->ready_toBounce_left = true;
				App->entityFactory->player->flipX = false;
				App->entityFactory->player->in_contact = true;
				if (type->type == COLLIDER_ENEMY && collider->type == COLLIDER_PLAYER)
				{
					dead = true;
				App->entityFactory->player->Die();
				}


			}
			else App->entityFactory->player->ready_toBounce_left = false;
			//if (check)
			velocity.x = MAX(velocity.x, distance); 
			
			check = false;
			
		}
	}


	/**/

	position.x += velocity.x;
	if(collider != nullptr)
		collider->rect.x = position.x;
}

void j1Entity::MovY()
{
	if (state != GOD && collider != nullptr)
	{
		Collider* type;
		float distance;


		if (velocity.y < 0)
		{
			
			distance = App->collision->DistanceToTopCollider(collider,type);
			
			velocity.y = MAX(velocity.y, distance);
			if (velocity.y == 0) target_speed.y = 0.0F;
			
		}
		else
		{
			 distance = App->collision->DistanceToBottomCollider(collider, type,ignore_platforms);
			velocity.y = MIN(velocity.y, distance);
			is_grounded = (distance == 0) ? true : false;
			if (distance == 0)
			{
				if (type->type == COLLIDER_PLAYER && collider->type == COLLIDER_ENEMY)
				{
					dead = true;
					App->entityFactory->player->Die();
				}


			}

		}

	}



	position.y += velocity.y;
	if (collider != nullptr)
		collider->rect.y = position.y; 

}

void j1Entity::SetPos(iPoint pos)
{
	
		position = pos;
		if (collider)
			collider->SetPos(position.x, position.y);

	
}

void j1Entity::Setid(int coin_counter)
{

	if (this->entity_type == E_TYPE::COLLECTABLE)
		id = coin_counter;
}

int j1Entity::GetId()
{
	if (this->entity_type == E_TYPE::COLLECTABLE)
		return id;

	return -1;
}




