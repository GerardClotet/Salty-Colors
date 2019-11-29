#ifndef __TEST_ENEMY__
#define __TEST_ENEMY__


#include "j1Enemy.h"
class TestEnemy : public j1Enemy
{

public:

	TestEnemy( iPoint position);
	~TestEnemy();


	
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	
	void SetPos(iPoint pos);
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;
	bool AwakeAttributes(pugi::xml_node config);

	
	void IdleUpdate();
	void MovingUpdate();
	void JumpingUpdate();
	void Jump();


	//bool Load(pugi::xml_node&);
	//bool Save(pugi::xml_node&) const;

	//Pathfinding variables

	//float chase_distance = 1000.0F;
	//int current_destination = 0;
	//int previous_destination = 0;
	//int next_destination = -1;
	//int jump_height = 2;
	//bool reached_X = false;
	//bool reached_Y = false;
	//bool current_is_grounded = false;
	//iPoint destination = { -1, -1 };




	//Pathfinding methods

	bool GetPath();
	virtual void ResetPathfindingVariables();


	void DrawPath();
	void PathfindingUpdate();
	void PathfindingPreupdate();
	void PathfindX();
	void PathfindY();

	bool Die();
	

	
private:
	//Animation e_test_IDLE;
	
};

#endif