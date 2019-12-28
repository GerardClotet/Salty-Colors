#include "Collectable.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1EntityFactory.h"
#include "j1Gui.h"

Collectable::Collectable(iPoint pos) : j1Entity(pos)
{
	AwakeAttributes(App->config);
	if (App->gui->GetAtlas() == nullptr)
	{
		entityTex = App->tex->Load(sprite_route.data());
		App->gui->SetAtlas(entityTex);
	}

	else entityTex = App->gui->GetAtlas();

	position = pos;

	animation_Coll = { 0,0,14,14 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_COLLECTABLE, App->entityFactory, true);

	collider->rect.x = position.x;
	collider->rect.y = position.y;
	entity_type = E_TYPE::COLLECTABLE;
}

Collectable::~Collectable()
{
}

bool Collectable::Start()
{
	return true;
}

bool Collectable::PreUpdate()
{

	currentAnimation = spin.GetCurrentFrame();

	return true;
}

void Collectable::Draw()
{
	if(entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation, 1.0f, flipX, false, 1.0f);
}

bool Collectable::CleanUp()
{
	return true;
}

bool Collectable::AwakeAttributes(pugi::xml_node config)
{

	sprite_route = PATH(config.child("entityFactory").child("collectable").child("folder").child_value(), config.child("entityFactory").child("collectable").child("sprite").child_value());

	for (auto node : config.child("entityFactory").child("collectable").child("animations").children("frame"))
	{
		spin.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	spin.loop = true;
	spin.speed = 10.0f;
	return true;
}

bool Collectable::Load(pugi::xml_node&)
{
	return false;
}

bool Collectable::Save(pugi::xml_node&) const
{
	return false;
}
