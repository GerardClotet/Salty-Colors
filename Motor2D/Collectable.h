

#ifndef _COLLECTABLE_H_
#define _COLLECTABLE_H_

#include "j1Entity.h"

class Collectable : public j1Entity
{

public:


	Collectable(iPoint pos);
	~Collectable();
	bool Start();
	bool PreUpdate();
	void Draw();
	bool CleanUp();
	bool AwakeAttributes(pugi::xml_node config);


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

	Animation spin;


};
#endif // !_COLLECTABLE_H_
