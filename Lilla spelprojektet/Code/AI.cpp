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
	rv = luaL_dofile(pathScript, "pathFinding.lua");

	targetScript = lua_open();
	OpenLuaLibs(targetScript);
	rv = luaL_dofile(targetScript, "targetFinding.lua");

	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	rv = luaL_dofile(spawnScript, "spawning.lua");

	return true;
}

void AI::findPath()
{
	lua_getglobal(spawnScript, "findPath");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla p� funktionen
	
	//h�mta v�rden

	//lua_pop(l, returncount); // Plocka bort returv�rden
}

void AI::findTarget()
{
	lua_getglobal(spawnScript, "findTarget");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla p� funktionen
	
	//h�mta v�rden

	//lua_pop(l, returncount); // Plocka bort returv�rden	
}

void AI::spawnEnemies()
{
	lua_getglobal(spawnScript, "spawnEnemies");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla p� funktionen
	
	//h�mta v�rden

	//lua_pop(l, returncount); // Plocka bort returv�rden
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
