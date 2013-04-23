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

	//pathScript = lua_open();
	//OpenLuaLibs(pathScript);
	//if(luaL_dofile(pathScript, scripts[0].c_str()))//pathfinding
	//	return false;



	//targetScript = lua_open();
	//OpenLuaLibs(targetScript);
	//if(luaL_dofile(targetScript, scripts[1].c_str())) //targetfinding
	//	return false;

	if(!initSpawnEnemies(scripts[2]))
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

	lua_newtable(spawnScript);


	//lua_pushnumber(l,inputnumber);

	lua_pcall(spawnScript, 1, 1, 0); //kalla på funktionen
	
	//hämta värden

	lua_pop(spawnScript, 1); // Plocka bort returvärden
}

bool AI::initSpawnEnemies(string scriptName)
{
	int enemiesPerMin = 10;


	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	if(luaL_dofile(spawnScript, scriptName.c_str())) //spawning
		return false;
	lua_getglobal(spawnScript,"init");

	//sendArray(arr, size, spawnedScript);
	lua_pushnumber(spawnScript,enemiesPerMin);


	lua_pcall(spawnScript, 2, 1, 0);
	int a = lua_tonumber(spawnScript, -1);
	
	
	lua_pop(spawnScript, 2);
	cout << "output: " << a << endl;

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

void AI::sendArray(int** arr, int size, lua_State* script)
{
	lua_newtable( script );

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			lua_pushnumber( script, i + j*size);
			lua_pushnumber( script, arr[i][j] );
			lua_rawset( script, -3 );			
		}
	
	}

	// set the number of elements (index to the last array element)
	lua_pushliteral( script, "n" );
	lua_pushnumber( script, size );
	lua_rawset( script, -3 );
}
