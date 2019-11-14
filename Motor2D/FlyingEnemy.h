#ifndef __Flying_ENEMY__
#define __Flying_ENEMY__


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




private:
	//Animation e_test_IDLE;

};

#endif