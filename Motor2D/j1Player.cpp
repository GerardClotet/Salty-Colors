#include "j1Render.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1EntityFactory.h"
#include "j1ParticleSystem.h"
j1Player::j1Player(iPoint pos) : j1Entity(pos)
{

	AwakeAttributes(App->config);
	entityTex = App->tex->Load(sprite_route.data());
	LOG("%s", sprite_route.data());
	position = pos;

	animation_Coll = { 0,0,24,42 };
	collider = App->collision->AddCollider(animation_Coll, COLLIDER_PLAYER, App->entityFactory, true);

	collider->rect.x = position.x;
	collider->rect.y = position.y;
	entity_type = E_TYPE::PLAYER;

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
	case BOUNCE: BounceUpdate();
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

	
		velocity.x = (target_speed.x * acceleration + velocity.x * (1 - acceleration)) * dt;
		velocity.y = (target_speed.y * acceleration + velocity.y * (1 - acceleration)) * dt;
	


	MovX();
	MovY();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (state != GOD) state = GOD;
		else state = IDLE;
	}
	

	
 
	
//	Draw();
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


void j1Player::IdleUpdate()
{
	target_speed.x = 0.0f;
	currentAnimation = player_IDLE.GetCurrentFrame();

	if (!lockInput)
		IdleActPool();

	
	if (!is_grounded && state != BOUNCE) state = JUMPING;

}

void j1Player::MovingUpdate()
{
	currentAnimation = player_RUN.GetCurrentFrame();
	if (startMove)
	{
		App->audio->PlayFx(App->scene->stepSFX, NO_REPEAT);
		startMove = false;
		stepSFXTimer.Start();
	}

	CheckWalkSound();

	if (!lockInput)
		MovingActPool();
		

	
	if (!is_grounded) state = JUMPING;

	
}
void j1Player::JumpingUpdate()
{
 	target_speed.y += gravity; // if targetspeed speed <0 ascending anim // if targetspeed >=0 falling anim
	if (target_speed.y < 0)
		currentAnimation = player_JUMP.GetCurrentFrame();

	if (target_speed.y >= 0 && target_speed.y < 8.0f)
			currentAnimation = player_MOMENTUM.GetCurrentFrame();

 		

	

	else if (target_speed.y  >= 10.0)
		currentAnimation = player_FALL.GetCurrentFrame();


	if (target_speed.y > fall_speed) 
		target_speed.y = fall_speed; //limit falling speed

	if (!lockInput)
		JumpActPool();

	

	
}

void j1Player::GodUpdate()
{
	currentAnimation = player_FALL.GetCurrentFrame();

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
		App->audio->SetVolume(0.0f);
		//App->entityFactory->DeleteAllEnemies();
	}

}

void j1Player::DashUpdate()
{
	currentAnimation = player_DASH.GetCurrentFrame();

	if (startDash)
	{
		velocity.y = 0;
		target_speed.y = 0;
		target_speed.x = 0;

		flipX ? target_speed.x = -movement_speed*7 : target_speed.x = movement_speed*7;
		init_distance = position.x;
		startDash = false;
	}


	distance = position.x - init_distance;
	
	
	if(abs(distance) >= DASH_DISTANCE || previous_pos == position.x)//when reach coll
	{

		if (!is_grounded)
			state = JUMPING;
		
		else if (is_grounded)
		{
			//App->particles->AddParticle(App->particles->test, position.x, position.y, { 1,1 });
			if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
			{
				state = MOVING;
				startMove = true;
			}

			else state = IDLE;
		}
	}

	previous_pos = position.x;
}
void j1Player::BounceUpdate()
{
	if (in_contact)
	{
		currentAnimation = player_WALL.GetCurrentFrame();
	}
	if (!in_contact)
	{
		currentAnimation = player_BOUNCE.GetCurrentFrame();
	}
	target_speed.y += gravity; // if targetspeed speed <0 ascending anim // if targetspeed >=0 falling anim

	if (target_speed.y > fall_speed)
		target_speed.y = fall_speed; //limit falling speed

	BounceActPool();
}



void j1Player::ResetPlayer()
{

	state = IDLE;

	velocity = { 0.0f, 0.0f };
	target_speed = { 0.0f, 0.0f };


	flipX = false;
	
}


void j1Player::IdleActPool()
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		state = DASH;
		App->audio->PlayFx(App->scene->dashSFX, NO_REPEAT);
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
		App->audio->PlayFx(App->scene->jumpSFX, NO_REPEAT);
		state = JUMPING;

	}

}

void j1Player::MovingActPool()
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		state = DASH;
		App->audio->PlayFx(App->scene->dashSFX, NO_REPEAT);
		startDash = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
	{
		state = IDLE;
		target_speed.x = 0.0F;
	}

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		target_speed.x = movement_speed * 1.5f;
		flipX = false;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed * 1.5f;
		flipX = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		target_speed.y = -jump_speed;
		is_grounded = false;
		App->audio->PlayFx(App->scene->jumpSFX, NO_REPEAT);
		state = JUMPING;
	}
}

bool j1Player::JumpActPool()
{
	if ((ready_toBounce_left || ready_toBounce_right) && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Bounce();
		state = BOUNCE;
		in_contact = false;
		return true;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && dashes != 0)
	{
		state = DASH;
		App->audio->PlayFx(App->scene->dashSFX, NO_REPEAT);
		startDash = true;
		dashes -= 1;
		return true;

	}
	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A) && state != BOUNCE)
		target_speed.x = 0.0f;

	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		target_speed.x = movement_speed * 1.5f;
		flipX = false;
	}

	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		target_speed.x = -movement_speed * 1.5f;
		flipX = true;
	}

	if (is_grounded)
		Ground();

	return true;
}

void j1Player::BounceActPool()
{
	if (is_grounded)
		Ground();
	

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Bounce();

	}


	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && dashes != 0)
	{
		state = DASH;
		App->audio->PlayFx(App->scene->dashSFX, NO_REPEAT);
		startDash = true;
		dashes -= 1;
	}
}



bool j1Player::Bounce()
{
	App->audio->PlayFx(App->scene->bounceSFX, NO_REPEAT);

	in_contact = false;
	if (flipX && ready_toBounce_right) //left
	{
		target_speed.y = -jump_speed;
		target_speed.x = -movement_speed * 2;
		LOG("BounceUpdate to left");
		ready_toBounce_right = false;
		return true;

	}
	if (!flipX && ready_toBounce_left) //right
	{
		target_speed.y = -jump_speed;
		target_speed.x = movement_speed * 2;
		LOG("BounceUpdate to right");
		ready_toBounce_left = false;
		return true;
	}
}

void j1Player::Ground()
{
	//App->particles->AddParticle(App->particles->test, position.x, position.y, { 1,1 });

	if (dashes < MAX_DASHES)
		dashes += 1;

	App->audio->PlayFx(App->scene->landSFX, NO_REPEAT);
	if (App->input->GetKey(SDL_SCANCODE_D) == App->input->GetKey(SDL_SCANCODE_A))
		state = IDLE;

	else
		state = MOVING;

	target_speed.y = 0.0F;
	velocity.y = 0.0F;
	LOG("grounded");
}


void j1Player::CheckWalkSound()
{
	if (stepSFXTimer.ReadMs() > STEP_TIME)
	{
		App->audio->PlayFx(App->scene->stepSFX, NO_REPEAT);
		stepSFXTimer.Start();
	}
}


bool j1Player::Load(pugi::xml_node&data)
{
	position.x = data.child("Player").attribute("x").as_int();
	position.y = data.child("Player").attribute("y").as_int();


	velocity.x = data.child("Player").child("velocity").attribute("x").as_float();
	velocity.y = data.child("Player").child("velocity").attribute("y").as_float();

	target_speed.x = data.child("Player").child("target_speed").attribute("x").as_float();
	target_speed.y = data.child("Player").child("target_speed").attribute("y").as_float();

	state = (PlayerState)data.child("Player").child("state").attribute("value").as_int();
	is_grounded = data.child("Player").child("is_grounded").attribute("value").as_bool();
	flipX = data.child("Player").child("flipX").attribute("value").as_bool();

	return true;
}

bool j1Player::Save(pugi::xml_node&data) const
{
	pugi::xml_node Ppos;


	Ppos = data.append_child("Player");

	Ppos.append_attribute("x") = position.x;
	Ppos.append_attribute("y") = position.y;


	Ppos.append_child("velocity").append_attribute("x") = velocity.x;
	Ppos.append_child("velocity").append_attribute("y") = velocity.y;

	Ppos.append_child("target_speed").append_attribute("x") = target_speed.x;
	Ppos.append_child("target_speed").append_attribute("y") = target_speed.y;


	Ppos.append_child("state").append_attribute("value") = state != DEAD ? (int)state : (int)IDLE;
	Ppos.append_child("is_grounded").append_attribute("value") = is_grounded;
	Ppos.append_child("flipX").append_attribute("value") = flipX;

	return true;
}


bool j1Player::AwakeAttributes(pugi::xml_node config)
{
	movement_speed = config.child("entityFactory").child("player").child("movement_speed").attribute("value").as_float();
	
	jump_speed = config.child("entityFactory").child("player").child("jump_speed").attribute("value").as_float();

	gravity = config.child("entityFactory").child("player").child("gravity").attribute("value").as_float();

	acceleration = config.child("entityFactory").child("player").child("acceleration").attribute("value").as_float();

	fall_speed = config.child("entityFactory").child("player").child("fall_speed").attribute("value").as_float();

	bounce_speed = config.child("entityFactory").child("player").child("bounce_speed").attribute("value").as_float();




	sprite_route = PATH(config.child("entityFactory").child("player").child("folder").child_value(), config.child("entityFactory").child("player").child("sprite").child_value());
	int animationID;
	for (auto node : config.child("entityFactory").child("player").child("animations").children("frame"))
	{
		animationID = node.attribute("id").as_int();

		if (animationID == 1)
			player_IDLE.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 2)
			player_RUN.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 3)
			player_JUMP.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 4)
			player_FALL.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });


		else if (animationID == 5)
			player_MOMENTUM.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 6)
			player_DASH.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 7)
			player_BOUNCE.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });

		else if (animationID == 8)
			player_WALL.PushBack({ node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() });
	}
	player_IDLE.loop = true;
	player_IDLE.speed = 10.0f;
	player_RUN.loop = true;
	player_RUN.speed = 10.0f;

	player_JUMP.loop = true;
	player_JUMP.speed = 5.0f;

	player_FALL.loop = true;
	player_FALL.speed = 5.0f;

	player_MOMENTUM.loop = true;
	player_MOMENTUM.speed = 5.0F;

	player_DASH.loop = true;
	player_DASH.speed = 10.0f;

	player_WALL.loop = true;
	player_WALL.speed = 10.0f;

	player_BOUNCE.loop = true;
	player_BOUNCE.speed = 20.0f;

	return true;
}

