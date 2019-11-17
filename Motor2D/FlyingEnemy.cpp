#include "j1Render.h"
#include "FlyingEnemy.h"
#include "j1EntityFactory.h"
#include "p2Log.h"




FlyingEnemy::FlyingEnemy(iPoint pos) : j1Enemy(pos) {

	AwakeAttributes(App->config);
	entityTex = App->tex->Load(sprite_route.data());

	LOG("%s", sprite_route.data());

	position = pos;
	animation_Coll = { 0,0, 40, 40 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_ENEMY);


}

FlyingEnemy::~FlyingEnemy()
{
}

bool FlyingEnemy::PreUpdate()
{
	return true;
}

bool FlyingEnemy::Update(float dt)
{

	currentAnimation = IDLE_Flying.GetCurrentFrame();

	return true;
}

bool FlyingEnemy::CleanUp()
{
	if (collider)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

void FlyingEnemy::SetPos(iPoint pos)
{
}

bool FlyingEnemy::Load(pugi::xml_node&)
{
	return true;
}

bool FlyingEnemy::Save(pugi::xml_node&) const
{
	return true;
}

bool FlyingEnemy::AwakeAttributes(pugi::xml_node config)
{
	sprite_route = PATH(config.child("entityFactory").child("FlyingEnemy").child_value(), config.child("entityFactory").child("FlyingEnemy").child("sprite").child_value());

	int animationID;

	for (auto node : config.child("entityFactory").child("FlyingEnemy").child("animations").child("frame"))
	{
		animationID = node.attribute("id").as_int();
		if (animationID == 1)
			IDLE_Flying.PushBack({ node.attribute("x").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}

	IDLE_Flying.loop = true;
	IDLE_Flying.speed = 10.0f;


	return true;
}

