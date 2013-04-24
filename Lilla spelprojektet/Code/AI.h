#pragma once
#include "stdafx.h"
#include <iostream>
#include "Structure.h"
#include "Node.h"

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
	{ NULL,         NULL }
};

using namespace std;

class AI
{
public:
	AI(void);
	~AI(void);

	bool init(Structure*** structures, Node** nodes ,string* scripts, int mapSize);
	void findPath(); // hanterar pathScript
	void findTarget(); // hanterar targetScript
	void spawnEnemies(); // hanterar spawnScript

	bool initSpawnEnemies(string scriptName, int mapSize);
	bool initFindPath(string scriptName, int mapSize);

	void sendArray(int** arr, int size, lua_State* script);

	void convertNodesToInt(int mapSize);
	void convertStructuresToInt(int mapSize);

	static void OpenLuaLibs(lua_State* l);

private:
	int** structuresInt;
	int** nodesInt;

	Structure*** structures;
	Node** nodes;
	lua_State* pathScript;
	lua_State* targetScript;
	lua_State* spawnScript;
};
