#include "AI.h"


AI::AI(void)
{
	structures = NULL;
	pathScript = NULL;
	targetScript = NULL;
	spawnScript = NULL;
}


AI::~AI(void)
{
}

bool AI::init(Structure*** structures)
{
	int rv = 0;
	this->structures = structures;

	pathScript = lua_open();
	OpenLuaLibs(pathScript);
	rv = luaL_dofile(pathScript, "test.lua");

	targetScript = lua_open();
	OpenLuaLibs(targetScript);
	rv = luaL_dofile(targetScript, "test.lua");

	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	rv = luaL_dofile(spawnScript, "test.lua");

	return true;
}

void AI::findPath()
{
	
}

void AI::findTarget()
{
	
}

void AI::spawnEnemies()
{
	
}

void AI::OpenLuaLibs(lua_State* l)
{
	const luaL_reg* lpLib;

	for(lpLib = lualibs; lpLib->func != NULL; lpLib++)
	{
		lpLib->func(l);
		lua_settop(l, 0);
	}
}
