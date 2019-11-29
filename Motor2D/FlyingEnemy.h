#ifndef __Flying_ENEMY__
#define __Flying_ENEMY__
#define POSITION_ERROR_Y 10.0F



#include "j1Enemy.h"
class FlyingEnemy : public j1Enemy
{



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


	 bool GetPath();
	 void ResetPathfindingVariables();
	 void DrawPath();
	 void PathfindingUpdate();
	 void PathfindingPreupdate();
	 void PathfindX();
	 void PathfindY();;


public:
	Animation IDLE_Flying;


private:
	//Animation e_test_IDLE;

};

#endif