#include "j1Render.h"
#include "j1Textures.h"
#include "TestEnemy.h"
#include "j1EntityFactory.h"
#include "p2Log.h"


TestEnemy::TestEnemy(iPoint pos) : j1Enemy(pos)
{
	AwakeAttributes(App->config);
	entityTex = App->tex->Load(sprite_route.data());
	//entityTex = App->tex->Load("textures/enemies/enemy.png");
	LOG("%s", sprite_route.data());
	position = pos;
	animation_Coll = { 0,0,52,41 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_ENEMY, App->entityFactory, true);
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	LOG("Created TestEnemy");
}

TestEnemy::~TestEnemy()
{
}


bool TestEnemy::PreUpdate()
{
	return true;
}

bool TestEnemy::Update(float dt)
{
	//velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	//velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;

	//MovX();//need to review this (this function only works for player, need to set one for enemies
	//MovY();

	Draw();
	return true;
}

void TestEnemy::Draw()
{
	if(entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation, 1.0f, flipX, false);
	//For some reason the texture isnt rendering
}

void TestEnemy::SetPos(iPoint pos)
{
	position = pos;
	if (collider)
		collider->SetPos(position.x, position.y);
}

bool TestEnemy::CleanUp()
{
	if (collider)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

bool TestEnemy::Load(pugi::xml_node&data)
{
	position.x = data.child("enemy").child("TestEnemy").attribute("x").as_int();
	position.y = data.child("Enemy").child("TestEnemy").attribute("y").as_int();

	flipX = data.child("enemy").child("TestEnemy").child("flipX").attribute("value").as_bool();

	return true;
}

bool TestEnemy::Save(pugi::xml_node&data) const
{
	pugi::xml_node e_data;

	e_data = data.append_child("enemy").child("TestEnemy");

	e_data.append_attribute("x") = position.x;
	e_data.append_attribute("y") = position.y;

	e_data.append_child("flipX").append_attribute("value") = flipX;

	return true;
}

bool TestEnemy::AwakeAttributes(pugi::xml_node config)
{
	//movement_speed = config.
	sprite_route = PATH(config.child("entityFactory").child("enemy").child("folder").child_value(), config.child("entityFactory").child("enemy").child("sprite").child_value());

	int animationID;
	for (auto node : config.child("entityFactory").child("enemy").child("animations").children("frame"))
	{
		animationID = node.attribute("id").as_int();
		if(animationID ==1)
			e_test_IDLE.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });


	}

	e_test_IDLE.loop = true;
	e_test_IDLE.speed = 5.0f;

	return true;
}
