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

bool AI::init(Structure*** structures, string* scripts)
{
	int rv = 0;
	this->structures = structures;

	pathScript = lua_open();
	OpenLuaLibs(pathScript);
	if(luaL_dofile(pathScript, scripts[0].c_str()))//pathfinding
		return false;

	targetScript = lua_open();
	OpenLuaLibs(targetScript);
	if(luaL_dofile(targetScript, scripts[1].c_str())) //targetfinding
		return false;

	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	if(luaL_dofile(spawnScript, scripts[2].c_str())) //spawning
		return false;
	

	cout << "the following scripts have been initiated:" << endl;
	cout << scripts[0] << endl << scripts[1] << endl << scripts[2] << endl;
	return true;
}

void AI::findPath()
{
	lua_getglobal(spawnScript, "findPath");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla på funktionen
	
	//hämta värden

	//lua_pop(l, returncount); // Plocka bort returvärden
}

void AI::findTarget()
{
	lua_getglobal(spawnScript, "findTarget");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla på funktionen
	
	//hämta värden

	//lua_pop(l, returncount); // Plocka bort returvärden	
}

void AI::spawnEnemies()
{
	lua_getglobal(spawnScript, "spawnEnemies");

	//lua_pushnumber(l,inputnumber);

	//lua_pcall(l, inputcount, returncount, 0); //kalla på funktionen
	
	//hämta värden

	//lua_pop(l, returncount); // Plocka bort returvärden
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
