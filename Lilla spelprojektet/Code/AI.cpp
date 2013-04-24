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


	//pathScript = lua_open();
	//OpenLuaLibs(pathScript);
	//if(luaL_dofile(pathScript, scripts[0].c_str()))//pathfinding
	//	return false;



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
			Enemy* tempE = new Enemy(D3DXVECTOR3(retVals[0],0,retVals[1]),1,0,5,0,0,0);
			enemies.push_back(tempE);			
		}

		lua_pop(spawnScript, 1);
	}
	lua_pop(spawnScript, 2);
	
	return enemies;
}

bool AI::initSpawnEnemies(string scriptName, int mapSize)
{
	int enemiesPerMin = 30;


	spawnScript = lua_open();
	OpenLuaLibs(spawnScript);
	if(luaL_dofile(spawnScript, scriptName.c_str())) //spawning
		return false;
	
	lua_getglobal(spawnScript,"init");

	convertNodesToInt(mapSize);
	sendArray(nodesInt, mapSize, 10 ,spawnScript);

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

void AI::sendArray(int** arr, int mapSize, int quadSize,lua_State* script)
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
