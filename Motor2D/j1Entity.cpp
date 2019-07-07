#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"


j1Entity::j1Entity(ENTITY_TYPE type, iPoint position)
{}

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
	if (entityTex != nullptr)
		App->render->Blit(entityTex, pos.x, pos.y, &currentAnimation.GetCurrentFrame()); //or animation

	/*else if(currentAnimation != nullptr)
		App->render->Blit()*/


}

bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&) const
{
	return true;
}