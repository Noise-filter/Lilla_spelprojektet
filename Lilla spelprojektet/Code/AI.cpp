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
	//delete both int 2Darrays

}

bool AI::init(Structure*** structures, Node** nodes, string* scripts,int mapSize)
{
	int rv = 0;
	this->structures = structures;
	this->nodes = nodes;

	structuresInt = new int*[mapSize-1];
	for(int i= 0; i < mapSize-1; i++)
		structuresInt[i] = new int[mapSize-1];

	nodesInt = new int*[mapSize];
	for(int i= 0; i < mapSize; i++)
		nodesInt[i] = new int[mapSize];

	if(!initSpawnEnemies(scripts[0], mapSize))
		return false;

	//targetScript = lua_open();
	//OpenLuaLibs(targetScript);
	//if(luaL_dofile(targetScript, scripts[1].c_str())) //targetfinding
	//	return false;

	if(!initSpawnEnemies(scripts[2],mapSize))
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

bool AI::initFindPath(string scriptName, int mapSize)
{
	pathScript = lua_open();
	OpenLuaLibs(pathScript);
	if(luaL_dofile(pathScript, scriptName.c_str()))//pathfinding
		return false;

	lua_getglobal(pathScript, "init");

	convertNodesToInt(mapSize);
	sendArray(nodesInt, mapSize, pathScript);

	lua_pushnumber(pathScript, mapSize);
	lua_pcall(pathScript, 2, 0, 0);

	int a = lua_tonumber(pathScript, -1);
	lua_pop(pathScript, 1);

	cout << "Output: " << a << endl;

	return true;
}

bool AI::initSpawnEnemies(string scriptName, int mapSize)
{
	int enemiesPerMin = 10;


	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	if(luaL_dofile(spawnScript, scriptName.c_str())) //spawning
		return false;
	lua_getglobal(spawnScript,"init");

	convertNodesToInt(mapSize);
	sendArray(nodesInt, mapSize, spawnScript);

	lua_pushnumber(spawnScript,enemiesPerMin);


	lua_pcall(spawnScript, 2, 1, 0);
	int a = lua_tonumber(spawnScript, -1);
	
	
	lua_pop(spawnScript, 1);
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

void AI::convertNodesToInt(int mapSize)
{
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			nodesInt[i][j] = nodes[i][j].getColor();
		}
	}
}

void AI::convertStructuresToInt(int mapSize)
{
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			structuresInt[i][j] = structures[i][j]->getRenderData().meshID;
		}
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
