#include "j1ParticleSystem.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1EntityFactory.h"
j1ParticleSystem::j1ParticleSystem()
{
	name.assign("particles_system");
}

j1ParticleSystem::~j1ParticleSystem()
{
}

bool j1ParticleSystem::Awake(pugi::xml_node& node)
{

	if (node.empty())
		LOG("emptynode");

	particleNode = node;
	return true;
}

bool j1ParticleSystem::Start()
{
	particles_tex = App->tex->Load("textures/particles/fire_01.png");
	ground_part_tex = App->tex->Load("textures/particles/groundfx.png");

	test.anim.PushBack({9, 5, 8, 22});
	test.anim.PushBack({ 29, 1, 17, 32 });

	
	test.anim.speed = 1.f;
	test.anim.loop = true;
	test.texture = ground_part_tex;


	return true;
}

bool j1ParticleSystem::Update(float dt)
{
	//if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	//{
	//	App->particles->AddParticle(App->particles->test, App->entityFactory->player->position.x, App->entityFactory->player->position.y, { 1,1 });
	//}


	return true;
}

bool j1ParticleSystem::PostUpdate()
{
	for (std::list<Particle*>::iterator p = active.begin(); p != active.end();)
	{

		if ((*p)->Update(App->GetDt()) == false)
		{
			//if (active[i]->deathParticle != nullptr)
			//	AddParticle(*active[i]->deathParticle, active[i]->position.x, //+ active[i]->impactPosition.x,
			//		active[i]->position.y, COLLIDER_ENEMY_SHOT);

			delete (*p);
			(*p) = nullptr;
			p = active.erase(p);
		}
		else if (SDL_GetTicks() >= (*p)->born)
		{
			App->render->Blit((*p)->texture, (*p)->position.x, (*p)->position.y, &(*p)->anim.GetCurrentFrame(), 1.0f, (*p)->renderFlip, (*p)->scale, (*p)->angle, (*p)->pivot.x * App->win->GetScale(), (*p)->pivot.y * App->win->GetScale());
			if ((*p)->fx_played == false && (*p)->fx != 0)
			{
				(*p)->fx_played = true;
				// Play particle fx here
				//App->audio->PlayFx((*p)->fx, 0);
			}
			++p;
		}
		else
			++p;
	}
	return true;
}

bool j1ParticleSystem::CleanUp()
{
	if (particles_tex)
	{
		App->tex->UnLoad(particles_tex);
		particles_tex = nullptr;
	}

	if (!active.empty())
	{
		std::list<Particle*>::iterator particles = active.begin();

		for (; particles != active.end();)
		{
			if ((*particles)->texture != nullptr)
			{
				App->tex->UnLoad((*particles)->texture);
				(*particles)->texture = nullptr;
			}
			delete (*particles);
			(*particles) = nullptr;
			particles = active.erase(particles);
		}
		active.clear();
	}
	return true;
}

void j1ParticleSystem::AddParticle(const Particle& particle, int x, int y, iPoint speed, Uint32 delay, SDL_RendererFlip rFlip , double angle, int pivotx, int pivoty, float scale)
{
	Particle* p = new Particle(particle);

	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;
	
	if (speed.x != 0 || speed.y != 0) //if we send specific speed, defines it
	{
		p->speed = speed;
	}
	p->renderFlip = rFlip;
	p->angle = angle;
	if (pivotx != INT_MAX && pivoty != INT_MAX)
	{
		p->pivot.x = pivotx;
		p->pivot.y = pivoty;
		p->position -= p->pivot;
	}

	p->scale = scale;
	active.push_back(p);
}

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed), fx(p.fx), born(p.born), life(p.life), texture(p.texture), renderFlip(p.renderFlip)

{
	
}

Particle::~Particle()
{

}

bool Particle::Update(float dt)
{

	bool ret = true;

	if (life > 0)
	{
		if ((SDL_GetTicks() - born) > life)
			ret = false;
	}

	//destroy particles respect camera position margins // TODO PARTICLES
	/*if (position.x > (abs(App->render->camera.x) / SCREEN_SIZE) + SCREEN_WIDTH - MARGIN)
		ret = false;
	else if (position.x < (abs(App->render->camera.x) / SCREEN_SIZE) - MARGIN - 150)
		ret = false;*/

		// destroy particle when animation is finished
	if (!anim.loop && anim.Finished())
		ret = false;

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	/*if (collider != nullptr)
		collider->SetPos(position.x, position.y);*/

	return ret;
	
}
