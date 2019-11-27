
#ifndef __j1PARTICLESYSTEM_H__
#define __j1PARTICLESYSTEM_H__


#include "j1Module.h"
#include "p2Animation.h"

#include "p2Point.h"
#include "SDL/include/SDL_render.h"
#define MAX_ACTIVE_PARTICLES 500

struct SDL_Texture;

enum COLLIDER_TYPE;


struct Particle // only dumb visual class
{
	//Collider* collider = nullptr;
	SDL_RendererFlip renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	double angle = 0;
	float scale = 1.F;
	iPoint pivot = { (int)INT_MAX, (int)INT_MAX };
	Animation anim;
	SDL_Texture* texture = nullptr;
	uint fx = 0u;
	iPoint position;
	//iPoint impactPosition = { NULL,NULL }; // TODO: to instantiate another particle on impact pos
	//bool impactPositioning = false;
	iPoint speed;
	Uint32 born = 0;
	Uint32 life = 0;
	//Uint32 damage = 0;
	bool fx_played = false;

	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update(float dt);
	// variables to instantiate collision particle
	/*Particle* onCollisionWallParticle = nullptr; // TODO:
	Particle* onCollisionGeneralParticle = nullptr;
	Particle* deathParticle = nullptr;*/

};


class j1ParticleSystem : public j1Module
{
public:
	j1ParticleSystem();
	~j1ParticleSystem();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	iPoint tt = { 0,0 };
	void AddParticle(const Particle& particle, int x, int y, iPoint speed, Uint32 delay =0, SDL_RendererFlip rFlip = SDL_RendererFlip::SDL_FLIP_NONE, double angle = 0, int pivotx = INT_MAX, int pivoty = INT_MAX, float scale = 1.0F);

	SDL_Texture* particles_tex;
	Particle test;
	SDL_Texture* ground_part_tex;
	
	std::list<Particle*> active;
	pugi::xml_node particleNode;
};

#endif