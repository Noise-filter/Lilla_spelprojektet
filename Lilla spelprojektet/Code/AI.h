#pragma once
#include "stdafx.h"
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
	{ NULL,         NULL }
};

using namespace std;

class AI
{
public:
	AI(void);
	~AI(void);

	bool init(Structure*** structures, Node** nodes ,string* scripts, int mapSize);
	vector<Waypoint> findPath(int start, int goal, int enemyType); // hanterar pathScript
	void findTarget(); // hanterar targetScript
	vector<Enemy*> spawnEnemies(float dt, int nrOfEnemies); // hanterar spawnScript

	bool initSpawnEnemies(string scriptName);
	bool initFindPath(string scriptName);
	bool initFindTarget(string scriptName);

	void sendArray(int** arr, int mapSize, lua_State* script);

	void convertNodesToInt();
	void convertStructuresToInt();

	static void OpenLuaLibs(lua_State* l);

private:
	int** structuresInt;
	int** nodesInt;
	int mapSize;
	int quadSize;

	Structure*** structures;
	Node** nodes;
	lua_State* pathScript;
	lua_State* targetScript;
	lua_State* spawnScript;
};