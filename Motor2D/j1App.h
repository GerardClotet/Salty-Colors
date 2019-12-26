#ifndef __j1APP_H__
#define __j1APP_H__

#include <list>
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"
#include "j1Timer.h"
// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1EntityFactory;
class j1Collision;
class j1MapChange;
class j1PathFinding;
class j1ParticleSystem;
class j1Gui;
class j1Fonts;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame(const char* file);
	void SaveGame(const char* file) const;
	void GetSaveGames(std::list<std::string>& list_to_fill) const;
	float GetDt();
	float GetInvers_AVG_FPS();

public:
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;
private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;
	
public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Map*				map;
	j1EntityFactory*	entityFactory;
	j1Collision*		collision;
	j1PathFinding*		pathfinding;
	j1MapChange*		swapScene;
	j1ParticleSystem*	particles;
	j1Gui*				gui;
	j1Fonts*			fonts;


	bool				vsync = false;
	bool				pause = false;
private:

	std::list<j1Module*>modules;
	uint				frames;
	//float				dt;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	std::string			load_game;
	mutable std::string	save_game;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				ms_timer;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	float				seconds_since_startup = 0.0f;

public:
	float				frame_rateCap = 0.0f;
	double				frame_rate;
	bool				capFrames = false;

};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif