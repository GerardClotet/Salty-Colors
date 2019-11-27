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



	

private:
	//Animation e_test_IDLE;
	
};

#endif