#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1EntityFactory.h"
j1Player::j1Player(iPoint position) : j1Entity(ENT_PLAYER, position)
{
	entityTex = App->tex->Load(App->entityFactory->sprite_route.data());
	LOG("%s", App->entityFactory->sprite_route.data());
	pos = position;
	currentAnimation = App->entityFactory->player_IDLE;
}

j1Player::~j1Player()
{
}

bool j1Player::Start()
{
	

	return true;
}

bool j1Player::Update(float dt)
{
	Draw();
	return true;
}

void j1Player::Draw()
{
	if (entityTex != nullptr)
		App->render->Blit(entityTex, pos.x, pos.y, &currentAnimation.GetCurrentFrame());
	LOG("player drawn");
}

