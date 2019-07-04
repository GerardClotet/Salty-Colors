#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2Defs.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1EntityFactory.h"
#include "PugiXml/src/pugixml.hpp"
j1Player::j1Player(ENTITY_TYPE type, pugi::xml_node, fPoint position, p2SString id, int clone_number) : j1Entity(type, position, id)
{
}

j1Player::~j1Player()
{
}
