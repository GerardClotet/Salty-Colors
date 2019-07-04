#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1EntityFactory.h"
#include "j1Window.h"
#include "p2Log.h"


j1Entity::j1Entity(ENTITY_TYPE type,pugi::xml_node config, fPoint position, p2SString id)
{
	this->type = type;

	sprite_route = PATH(config.child("folder").child_value, config.child("sprite").child_value());
	entityTex = App->tex->Load(sprite_route.GetString());
	this->position = position;
	

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
		App->render->Blit(entityTex, position.x, position.y, &entityRect); //or animation


}

void j1Entity::LoadAnimations(pugi::xml_node config)
{
	currentAnimation = new Animation[config.child("animations").attribute("count").as_int(1)];

	int index = 0;
	pugi::xml_node animation;
	for (animation = config.child("animations").first_child(); animation; animation = animation.next_sibling())
	{
		pugi::xml_node frame;
		for (frame = animation.child("frame"); frame; frame = frame.next_sibling("frame")) {
			currentAnimation[index].speed = animation.attribute("speed").as_float();
			currentAnimation[index].loop = animation.attribute("loops").as_bool(true);
			index++;
		}
		entityRect = { 0,0,config.child("collider").attribute("width").as_int(), config.child("collider").attribute("height").as_int() };

	}
}


bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&) const
{
	return true;
}