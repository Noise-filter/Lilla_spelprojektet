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

	initFindPath(pathScript, structures, 10);

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

	lua_newtable(spawnScript);


	//lua_pushnumber(l,inputnumber);

	lua_pcall(spawnScript, 1, 1, 0); //kalla p� funktionen
	
	//h�mta v�rden

	lua_pop(spawnScript, 1); // Plocka bort returv�rden
}

bool AI::initFindPath(lua_State* l, Structure*** structures, int mapSize)
{
	lua_getglobal(pathScript, "init");

	lua_newtable(pathScript); 

	lua_pushnumber(pathScript, 1);
	lua_pushnumber(pathScript, 34);
	lua_rawset(pathScript, -3);

	lua_pushnumber(pathScript, 2);
	lua_pushnumber(pathScript, 32);
	lua_rawset(pathScript, -3);

	lua_pushliteral(pathScript, "n");
	lua_pushnumber(pathScript, 2);
	lua_rawset(pathScript, -3);
	
	lua_pcall(pathScript, 1, 0, 0);
	return true;
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
