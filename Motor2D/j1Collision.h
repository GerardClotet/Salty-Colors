#ifndef __J1COLLISION_H__
#define __J1COLLISION_H__

#include "j1Module.h"
enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_FLOOR,
	COLLIDER_PLATFORM,
	COLLIDER_PLAYER,
	COLLIDER_TRIGGER,
	COLLIDER_ENEMY,
	COLLIDER_FLYING,
	COLLIDER_COLLECTABLE,
	COLLIDER_MAX,
};

struct Collider

{
	SDL_Rect rect;
	bool to_delete = false;
	ColliderType type;
	j1Module* callback = nullptr;

	Collider() {}
	Collider(const SDL_Rect& rectangle, const ColliderType& type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}
	~Collider()
	{
		callback = nullptr;
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void SetShape(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	void SetType(ColliderType type)
	{
		this->type = type;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:
	j1Collision();
	~j1Collision();

	bool Update(float dt) override;
	bool CleanUp() override;
	bool PreUpdate();
	bool Awake(pugi::xml_node&);
	bool Start();

	float DistanceToRightCollider(Collider* coll, Collider*& colltype) const;
	float DistanceToLeftCollider(Collider* coll, Collider* &colltype) const;
	float DistanceToTopCollider(Collider* coll, Collider*& colltype) const;
	float DistanceToBottomCollider(Collider* coll, Collider*& colltype, bool ignore_platforms = false ) const;
	Collider* AddCollider(const SDL_Rect& rect, const ColliderType& type, j1Module* callback = nullptr, const bool& player = false);

	void DebugDraw();
	uint max_colliders = 0;

	bool Triggercolliding = false;

private:

	Collider** colliders = nullptr;
	Collider* player_collider = nullptr;
	bool debug = false;

};
#endif