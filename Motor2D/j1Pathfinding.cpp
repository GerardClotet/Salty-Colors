#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"
j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t < 2;
}

bool j1PathFinding::HasGroundBelow(const iPoint& pos) const
{
	bool ret = false;

	for (int i = pos.y; i < height; i++)
	{
		if (IsGround({ pos.x,i }))
			return true;
	}

	return ret;
}

bool j1PathFinding::IsGround(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

bool j1PathFinding::IsPlatform(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t == 1;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, bool flier) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	if (flier)
	{
		// north_west
		cell.create(pos.x - 1, pos.y - 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// north_east
		cell.create(pos.x + 1, pos.y - 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// south_west
		cell.create(pos.x - 1, pos.y + 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));

		// south_east
		cell.create(pos.x + 1, pos.y + 1);
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, cell, this));
	}


	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination, int jump_length)
{
	if (jump_length > 0)
	{
		g = parent->g + 1 + jump_length / 4;
	}
	else
		g = parent->g + 1;
	h = pos.DistanceNoSqrt(destination);

	this->jump_length = jump_length;

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, int maxCharacterJumpHeight)
{
	
	last_path.Clear();
	bool flier = maxCharacterJumpHeight > 0 ? false : true;

	// TODO 1: if origin or destination are not walkable, return -1
	if (!IsWalkable(destination) || (!flier && !HasGroundBelow(destination))) return -1;

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList close;

	PathNode node_origin = PathNode(0, origin.DistanceNoSqrt(destination), origin, nullptr);
	open.list.add(node_origin);



	if (!flier)
	{
		if (IsGround({ origin.x, origin.y + 1 }))
			node_origin.jump_length = 0;
		else
			node_origin.jump_length = maxCharacterJumpHeight * 2;
	}
	else
		node_origin.jump_length = 0;

	open.list.add(node_origin);

	while (open.list.count() > 0)
	{
		p2List_item<PathNode>* current_node = open.GetNodeLowestScore();
		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.add(current_node->data);


		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (close.list.end->data.pos == destination)
		{
			const PathNode* node = nullptr;
			for (node = &current_node->data; node->pos != origin; node = node->parent) {
				last_path.PushBack(node->pos);
			}
			last_path.PushBack(node->pos);
			last_path.Flip();
			break;
		}
		else
		{
			// TODO 5: Fill a list of all adjancent nodes
			PathList childs;
			close.list.end->data.FindWalkableAdjacents(childs, flier);

			// TODO 6: Iterate adjancent nodes:					
			p2List_item<PathNode>* child_node = childs.list.start;
			while (child_node != NULL)
			{
				bool on_ground = false;

				if (!close.Find(child_node->data.pos))	// ignore nodes in the closed list
				{
					int child_jump_length = 0;
					if (!flier)
					{
						if (IsGround({ child_node->data.pos.x, child_node->data.pos.y + 1 }))
							on_ground = true;

						int parent_jump_length = current_node->data.jump_length;
						child_jump_length = parent_jump_length;

						if (on_ground)
							child_jump_length = 0;
						else if (child_node->data.pos.y < current_node->data.pos.y)
						{
							if (parent_jump_length % 2 == 0)
								child_jump_length = parent_jump_length + 2;
							else
								child_jump_length = parent_jump_length + 1;
						}
						else if (child_node->data.pos.y > current_node->data.pos.y)
						{
							if (parent_jump_length % 2 == 0)
								child_jump_length = MAX(maxCharacterJumpHeight * 2, parent_jump_length + 2);
							else
								child_jump_length = MAX(maxCharacterJumpHeight * 2, parent_jump_length + 1);

							if (IsPlatform({ child_node->data.pos.x, child_node->data.pos.y }))
							{
								child_node = child_node->next;
								continue;
							}
						}
						else if (!on_ground && child_node->data.pos.x != current_node->data.pos.x)
							child_jump_length = parent_jump_length + 1;

						if (parent_jump_length % 2 != 0 && current_node->data.pos.x != child_node->data.pos.x)
						{
							child_node = child_node->next;
							continue;
						}
						if (parent_jump_length >= maxCharacterJumpHeight * 2 && current_node->data.pos.y > child_node->data.pos.y)
						{
							child_node = child_node->next;
							continue;
						}
					}
					child_node->data.CalculateF(destination, child_jump_length); // If it is NOT found, calculate its F and add it to the open list

					if (open.Find(child_node->data.pos)) // If it is already in the open list, check if it is a better path (compare G)
					{
						if (open.Find(child_node->data.pos)->data.g > child_node->data.g)
						{
							// If it is a better path, Update the parent
							// replace old node at open
							open.Find(child_node->data.pos)->data.g = child_node->data.g;
							open.Find(child_node->data.pos)->data.parent = child_node->data.parent;
						}
					}
					else open.list.add(child_node->data);
				}
				child_node = child_node->next;
			}
		}
		open.list.del(current_node);
	}


	return -1;
}
