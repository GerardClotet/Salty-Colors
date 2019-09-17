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

	animation_Coll = { 0,0,24,42 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_PLAYER, App->entityFactory, true);

	collider->rect.x = position.x;
	collider->rect.y = position.y;

}

j1Player::~j1Player()
{
}

bool j1Player::Start()
{
	

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
	case DASH:
		DashUpdate();
		break;
	case DEAD:
		break;
	default:
		break;
	}

	/*velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration))*dt;
	velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration))*dt;*/

	
	return true;

}

bool j1Player::Update(float dt)
{

	switch (state)
	{
	case JUMPING:
		target_speed.y += gravity*dt;
		if (target_speed.y > fall_speed)
			target_speed.y = fall_speed; //limit falling speed
		break;

		
	}
	velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
	velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;

	MovX();
	MovY();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (state != GOD) state = GOD;
		else state = IDLE;
	}
	

	
	
	Draw();
	if(state != DEAD)
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
	target_speed.x = 0.0f;
	currentAnimation = App->entityFactory->player_IDLE.GetCurrentFrame();

	if (!lockInput)

	{
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			state = DASH;
			App->audio->PlayFx(App->scene->dashSFX, 0);
			startDash = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
		{
			state = MOVING;
			startMove = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			target_speed.y = -jump_speed;
			is_grounded = false;
			App->audio->PlayFx(App->scene->jumpSFX, 0);
			state = JUMPING;

		}
	}
	if (!is_grounded) state = JUMPING;

}

void j1Player::MovingUpdate()
{
	currentAnimation = App->entityFactory->player_RUN.GetCurrentFrame();
	if (startMove)
	{
		App->audio->PlayFx(App->scene->stepSFX, 0);
		startMove = false;
		stepSFXTimer.Start();
	}

	if (stepSFXTimer.ReadMs() > 250.0f)
	{
		App->audio->PlayFx(App->scene->stepSFX, 0);
		stepSFXTimer.Start();
	}

	if (!lockInput)
	{

		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			state = DASH;
			App->audio->PlayFx(App->scene->dashSFX, 0);
			startDash = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
		{
			state = IDLE;
			target_speed.x = 0.0F;
		}

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			target_speed.x = movement_speed*1.5f;
			flipX = false;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			target_speed.x = -movement_speed*1.5f;
			flipX = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			target_speed.y = -jump_speed;
			is_grounded = false;
			App->audio->PlayFx(App->scene->jumpSFX, 0);
			state = JUMPING;
		}
	}
	if (!is_grounded) state = JUMPING;

	
}

void j1Player::JumpingUpdate()
{
 	//target_speed.y += gravity; // if targetspeed speed <0 ascending anim // if targetspeed >=0 falling anim
	if (target_speed.y < 0)
		currentAnimation = App->entityFactory->player_JUMP.GetCurrentFrame();

	if (target_speed.y >= 0 && target_speed.y < 8.0f)
	{
		
			currentAnimation = App->entityFactory->player_MOMENTUM.GetCurrentFrame();

 		

	}

	else if (target_speed.y  >= 10.0)
		currentAnimation = App->entityFactory->player_FALL.GetCurrentFrame();


	//if (target_speed.y > fall_speed) 
	//	target_speed.y = fall_speed; //limit falling speed

	if (!lockInput)
	{
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && dashes !=0)
		{
			state = DASH;
			App->audio->PlayFx(App->scene->dashSFX, 0);
			startDash = true;
			dashes -= 1;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
			target_speed.x = 0.0f;

		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			target_speed.x = movement_speed*1.5f;
			flipX = false;
		}

		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			target_speed.x = -movement_speed*1.5f;
			flipX = true;
		}
	}
		if (is_grounded)
		{
			if( dashes < MAX_DASHES)
				dashes += 1;

			App->audio->PlayFx(App->scene->landSFX, 0);
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
		target_speed.x = movement_speed*5;
		flipX = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed*5;
		flipX = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == App->input->GetKey(SDL_SCANCODE_S)) target_speed.y = 0.0F;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) target_speed.y = -movement_speed*5;
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) target_speed.y = movement_speed*5;

}

void j1Player::Die()
{
	if (position.y > App->map->data.height * App->map->data.tile_height && state != DEAD && state != GOD)
	{
 		state = DEAD;
		Mix_PausedMusic();
		App->scene->ReLoadLevel();
		App->audio->SetVolume(0);
	}

}

void j1Player::DashUpdate()
{
	currentAnimation = App->entityFactory->player_DASH.GetCurrentFrame();
	LOG("velocity %f x", velocity.x);
	if (!flipX && startDash)
	{
		velocity.y = 0;
		target_speed.y = 0;
		target_speed.x = 0;
	//	velocity.x = movement_speed*5000000000;
		target_speed.x = movement_speed * 20;

		LOG("velocity %f x", velocity.x);
		startDash = false;
	}
	else if (flipX && startDash)
	{
		velocity.y = 0;
		target_speed.y = 0;
		target_speed.x = 0;
		
		velocity.x = -movement_speed * 50 * (1 / App->GetDt());
		target_speed.x = - movement_speed * 20;

		LOG("velocity %f x", velocity.x);
		startDash = false;
	}
	LOG("targetSpeed %f x", target_speed.x);

	LOG("velocity %f x", velocity.x);
	if (velocity.x > -10 && flipX == true || velocity.x < 10 && flipX == false)
	{
		if (!is_grounded)
		{
			state = JUMPING;
		}
		else if (is_grounded)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
			{
				state = MOVING;
				startMove = true;
			}

			else state = IDLE;
		}
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


	return true;
}
