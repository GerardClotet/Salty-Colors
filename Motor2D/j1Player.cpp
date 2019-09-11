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
j1Player::j1Player(iPoint pos) : j1Entity(ENT_PLAYER, pos)
{

	LoadAttributes(App->config);
	entityTex = App->tex->Load(App->entityFactory->sprite_route.data());
	LOG("%s", App->entityFactory->sprite_route.data());
	position = pos;
	//currentAnimation = App->entityFactory->player_IDLE;

	animation_Coll = { 0,0,24,42 };
	coll_offSet = 13;
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_PLAYER, App->entityFactory, true);

	collider->rect.x = position.x;
	collider->rect.y = position.y;// +coll_offSet;

}

j1Player::~j1Player()
{
}

bool j1Player::Start()
{
	
	//collider = App->collision->AddCollider;

	return true;
}

bool j1Player::PreUpdate()
{
	switch (state)
	{
	case NO_STATE:
		break;
	case IDLE: IdleUpdate();
		break;
	case MOVING: MovingUpdate();
		break;
	case JUMPING: JumpingUpdate();
		break;
	case GOD: GodUpdate();
		break;
	case DEAD:
		break;
	default:
		break;
	}

	velocity.x = target_speed.x * acceleration + velocity.x * (1 - acceleration);
	velocity.y = target_speed.y * acceleration + velocity.y * (1 - acceleration);

	/*if (!is_grounded && state != GOD)
		state = JUMPING;*/
	return true;

}

bool j1Player::Update(float dt)
{
	MovX();
	MovY();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (state != GOD) state = GOD;
		else state = IDLE;
	}

	
	
	Draw();

	Die();

	return true;
}

void j1Player::Draw()
{
	if (entityTex != nullptr)
		App->render->Blit(entityTex, position.x, position.y, &currentAnimation,1.0f,flipX,false, spriteIncrease);
}

bool j1Player::CleanUp()
{
	
	if (collider)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

	if (!is_grounded) state = JUMPING;

	return true;
}

void j1Player::SetPos(iPoint pos)
{
	position = pos;
	if (collider)
		collider->SetPos(position.x, position.y);

	state = JUMPING;
}

void j1Player::IdleUpdate()
{
	//target_speed.y += gravity; //need to see isgrounded funct
	target_speed.x = 0.0f;
	currentAnimation = App->entityFactory->player_IDLE.GetCurrentFrame();

	if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
		state = MOVING;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		target_speed.y = -jump_speed;
		is_grounded = false;
		state = JUMPING;

		//jump sfx
	}
	if (!is_grounded) state = JUMPING;

}

void j1Player::MovingUpdate()
{
	currentAnimation = App->entityFactory->player_RUN.GetCurrentFrame();

	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
	{
		state = IDLE;
		target_speed.x = 0.0F;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		target_speed.x = movement_speed;
		flipX = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed;
		flipX = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		target_speed.y = -jump_speed;
		is_grounded = false;
		state = JUMPING;
	}

	if (!is_grounded) state = JUMPING;

	
}

void j1Player::JumpingUpdate()
{
 	target_speed.y += gravity; // if targetspeed speed <0 ascending anim // if targetspeed >=0 falling anim
	if (target_speed.y < 0)
		currentAnimation = App->entityFactory->player_JUMP.GetCurrentFrame();

	if (target_speed.y >= 0 && target_speed.y < 10.0f)
	{
		
			currentAnimation = App->entityFactory->player_MOMENTUM.GetCurrentFrame();

 		

	}

	else if (target_speed.y  >= 10.0)
		currentAnimation = App->entityFactory->player_FALL.GetCurrentFrame();


	if (target_speed.y > fall_speed) 
		target_speed.y = fall_speed; //limit falling speed

	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
		target_speed.x = 0.0f;

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		target_speed.x = movement_speed;
		flipX = false;
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed;
		flipX = true;
	}

	if (is_grounded)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
			state = IDLE;

		else
			state = MOVING;

		target_speed.y = 0.0F;
		velocity.y = 0.0F;
		LOG("grounded");
	}
}

void j1Player::GodUpdate()
{
	currentAnimation = App->entityFactory->player_FALL.GetCurrentFrame();

	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A)) target_speed.x = 0.0F;
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		target_speed.x = movement_speed;
		flipX = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed;
		flipX = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == App->input->GetKey(SDL_SCANCODE_S)) target_speed.y = 0.0F;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) target_speed.y = -movement_speed;
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) target_speed.y = movement_speed;

}

void j1Player::Die()
{
	if (position.y > App->map->data.height * App->map->data.tile_height && state != DEAD && state != GOD)
	{
		state = DEAD;
		//App->entityFactory->playerActive = false;
		
		App->scene->ReLoadLevel();
		ResetPlayer();

	}

}

void j1Player::ResetPlayer()
{

	state = IDLE;

	velocity = { 0.0f, 0.0f };
	target_speed = { 0.0f, 0.0f };
	flipX = false;

}



bool j1Player::LoadAttributes(pugi::xml_node config)
{
	movement_speed = config.child("entityFactory").child("player").child("movement_speed").attribute("value").as_float();
	
	jump_speed = config.child("entityFactory").child("player").child("jump_speed").attribute("value").as_float();

	gravity = config.child("entityFactory").child("player").child("gravity").attribute("value").as_float();

	acceleration = config.child("entityFactory").child("player").child("acceleration").attribute("value").as_float();

	fall_speed = config.child("entityFactory").child("player").child("fall_speed").attribute("value").as_float();

	threshold = config.child("entityFactory").child("player").child("threshold").attribute("value").as_float();

	return true;
}
