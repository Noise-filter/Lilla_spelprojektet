#pragma once
#include <iostream>
#include "Structure.h"
#include "Node.h"
#include "Enemy.h"
#include <vector>
extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}
static const luaL_reg lualibs[] =
{
	{ "base",       luaopen_base },
	{ "math",       luaopen_math },
	{ "table",		luaopen_table },
	{ "os",			luaopen_os	},
	{ NULL,         NULL }
};

using namespace std;

class AI
{
public:
	AI(void);
	~AI(void);

	bool init(Structure*** structures, Node** nodes ,string* scripts, int mapSize, int quadSize, int enemiesPerMin, int difficulty);
	vector<Waypoint> findPath(Waypoint start, Waypoint goal, int enemyType); // hanterar pathScript
	vector<float> findTarget(Waypoint pos, int type); // hanterar targetScript
	vector<Enemy*> spawnEnemies(float dt, int nrOfEnemies); // hanterar spawnScript

	bool initSpawnEnemies(string scriptName);
	bool initFindPath(string scriptName);
	bool initFindTarget(string scriptName);

	void sendArray(int** arr, int mapSize, lua_State* script);

	void convertNodesToInt();
	void convertStructuresToInt();

	Structure* getStrucutre(int x, int y);

	static void OpenLuaLibs(lua_State* l);

private:
	int** structuresInt;
	int** nodesInt;
	int mapSize;
	int quadSize;

	int difficulty;
	int enemiesPerMin;

	Structure*** structures;
	Node** nodes;
	lua_State* pathScript;
	lua_State* targetScript;
	lua_State* spawnScript;
};