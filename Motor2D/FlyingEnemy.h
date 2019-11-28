#ifndef __Flying_ENEMY__
#define __Flying_ENEMY__
#define POSITION_ERROR_Y 10.0F



#include "j1Enemy.h"
class FlyingEnemy : public j1Enemy
{

public:
	bool moving_down = false;
	float chase_distance = 1000.0F;
	int current_destination = 0;
	int previous_destination = 0;
	int next_destination = -1;
	int jump_height = 2;
	bool reached_X = false;
	bool reached_Y = false;
	bool current_is_grounded = false;
	iPoint destination = { -1, -1 };
	Animation IDLE_Flying;

public:

	FlyingEnemy(iPoint position);
	~FlyingEnemy();



	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	void SetPos(iPoint pos);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
	bool AwakeAttributes(pugi::xml_node config);

	void MovingUpdate();


	virtual bool GetPath();
	virtual void ResetPathfindingVariables();
	virtual void DrawPath();
	virtual void PathfindingUpdate();
	virtual void PathfindingPreupdate();
	virtual void PathfindX();
	virtual void PathfindY();;



private:
	//Animation e_test_IDLE;

};

#endif