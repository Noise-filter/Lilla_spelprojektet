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
	for(int i = 0; i < mapSize; i++)
	{
		SAFE_DELETE_ARRAY(nodesInt[i]);
	}
	SAFE_DELETE_ARRAY(nodesInt);


	for(int i = 0; i < mapSize-1; i++)
	{
		SAFE_DELETE_ARRAY(structuresInt[i]);
	}
	SAFE_DELETE_ARRAY(structuresInt);

	lua_close(pathScript);
	//lua_close(targetScript);
	lua_close(spawnScript);
}

bool AI::init(Structure*** structures, Node** nodes, string* scripts,int mapSize, int quadSize)
{
	int rv = 0;
	this->structures = structures;
	this->nodes = nodes;
	this->mapSize = mapSize;
	this->quadSize = quadSize;

	structuresInt = new int*[mapSize-1];
	for(int i= 0; i < mapSize-1; i++)
		structuresInt[i] = new int[mapSize-1];

	nodesInt = new int*[mapSize];
	for(int i= 0; i < mapSize; i++)
		nodesInt[i] = new int[mapSize];

	if(!initFindPath(scripts[0]))
		return false;

	if(!initFindTarget(scripts[1]))
		return false;

	if(!initSpawnEnemies(scripts[2]))
		return false;

	cout << "the following scripts have been initiated:" << endl;
	cout << scripts[0] << endl << scripts[1] << endl << scripts[2] << endl;

	return true;
}

vector<Waypoint> AI::findPath(int start, int goal, int enemyType)
{
	vector<Waypoint> wayPoints;

	lua_getglobal(pathScript, "astar");

	lua_pushnumber(pathScript, start);
	lua_pushnumber(pathScript, goal);
	lua_pushnumber(pathScript, enemyType);

	lua_pcall(pathScript, 3, 2, 0); //kalla på funktionen

	//hämta värden
	int a = lua_tonumber(pathScript, -1);
	lua_pop(pathScript, 1);

	if(a > 0)
	{
		//Hämta ut tabellen
		lua_pushnil(pathScript);
		while(lua_next(pathScript, -2) != 0)
		{
			int temp = lua_tonumber(pathScript, -1);
			wayPoints.push_back(Waypoint(((int)temp % mapSize), ((int)temp / mapSize)));
			//cout << "A*: " << temp << endl;
			lua_pop(pathScript, 1);
		}
		lua_pop(pathScript, 1);
	}

	return wayPoints;
}

vector<float> AI::findTarget()
{
	vector<float> target;

	lua_getglobal(targetScript, "findTarget");

	sendArray(structuresInt, mapSize-1, targetScript);
	lua_pushnumber(targetScript, 10);
	lua_pushnumber(targetScript, 5);
	lua_pushnumber(targetScript, 1);
	
	lua_pcall(targetScript, 4, 2, 0); //kalla på funktionen
	
	float targetY = lua_tonumber(targetScript, -1);
	lua_pop(targetScript, 1);
	float targetX = lua_tonumber(targetScript, -1);
	lua_pop(targetScript, 1);

	//hämta värden
	target.push_back(targetX);
	target.push_back(targetY);
	//cout<< "X = "<<targetX<<" Y = "<<targetY<<endl;

	return target;
}

vector<Enemy*> AI::spawnEnemies(float dt, int nrOfEnemies)
{
	vector<Enemy*> enemies;
	Enemy* tempE;
	int retVals[4];
	int counter = 0;
	lua_getglobal(spawnScript, "spawning");
	lua_pushnumber(spawnScript,dt);
	lua_pushnumber(spawnScript,nrOfEnemies);

	lua_pcall(spawnScript, 2, 2, 0); //kalla på funktionen
	
	//hämta värden
	int spawnedEnemies = lua_tonumber(spawnScript,-1);
	if(spawnedEnemies > 0)
	{
		cout << "#enemies: " << spawnedEnemies << endl;
	}

	lua_pushnil(spawnScript);
	while (lua_next(spawnScript, 1) != 0)
	{
		counter = 0;
		lua_pushnil(spawnScript);
		while (lua_next(spawnScript, -2) != 0)
		{
			if( lua_tonumber(spawnScript, -1) > -1)
			{
				retVals[counter++] = lua_tonumber(spawnScript, -1);
				cout << lua_typename(spawnScript, lua_type(spawnScript, -2)) << ": value"<< lua_tonumber(spawnScript, -1) << endl;
			}
			lua_pop(spawnScript, 1);	
		}
		if(spawnedEnemies > 0)
		{
			Enemy* tempE = new Enemy(D3DXVECTOR3(retVals[0],0,retVals[1]),1,0,5,0,30,0);
			enemies.push_back(tempE);
		}

		lua_pop(spawnScript, 1);
	}
	lua_pop(spawnScript, 2);
	
	return enemies;
}

bool AI::initFindPath(string scriptName)
{
	pathScript = lua_open();
	OpenLuaLibs(pathScript);
	if(luaL_dofile(pathScript, scriptName.c_str()))//pathfinding
		return false;

	lua_getglobal(pathScript, "init");

	convertNodesToInt();
	sendArray(nodesInt, mapSize, pathScript);

	lua_pushnumber(pathScript, mapSize);
	lua_pcall(pathScript, 2, 0, 0);

	return true;
}

bool AI::initSpawnEnemies(string scriptName)
{
	int enemiesPerMin = 10;


	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	if(luaL_dofile(spawnScript, scriptName.c_str())) //spawning
		return false;
	
	lua_getglobal(spawnScript,"init");

	convertNodesToInt();
	sendArray(nodesInt, mapSize ,spawnScript);

	lua_pushnumber(spawnScript,enemiesPerMin);


	lua_pcall(spawnScript, 2, 1, 0);
	int a = lua_tonumber(spawnScript, -1);
	
	
	lua_pop(spawnScript, 1);
	cout << "output: " << a << endl;

	return true;
}

bool AI::initFindTarget(string scriptName)
{
	targetScript = lua_open();
	OpenLuaLibs(targetScript);
	if(luaL_dofile(targetScript, scriptName.c_str()))
		return false;

	lua_getglobal(targetScript, "init");

	convertStructuresToInt();
	//sendArray(structuresInt, mapSize-1, targetScript);

	lua_pushnumber(targetScript, mapSize);
	lua_pcall(targetScript, 1, 1, 0);

	int a = lua_tonumber(targetScript, -1);
	lua_pop(targetScript, 1);

	//cout << "Output: " << a << endl;

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

void AI::convertNodesToInt()
{
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
				nodesInt[i][j] = nodes[i][j].getColor();
		}
	}
}

void AI::convertStructuresToInt()
{
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				structuresInt[i][j] = structures[i][j]->getRenderData().meshID;
			}
			else
			{
				structuresInt[i][j] = 0;
			}
		}
	}
}

void AI::sendArray(int** arr, int mapSize, lua_State* script)
{
	lua_newtable( script );

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			lua_pushnumber( script, i + j*mapSize);
			lua_pushnumber( script, arr[i][j] );
			lua_rawset( script, -3 );			
		}
	
	}

	// set the number of elements (index to the last array element)
	lua_pushliteral( script, "n" );
	lua_pushnumber( script, mapSize );
	lua_rawset( script, -3 );

	lua_pushliteral( script, "q" );
	lua_pushnumber( script, quadSize);
	lua_rawset( script, -3 );
}
