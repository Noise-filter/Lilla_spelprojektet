#pragma once
#include "stdafx.h"
#include <iostream>
#include "Structure.h"


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

	bool init(Structure*** structures);
	void findPath(); // hanterar pathScript
	void findTarget(); // hanterar targetScript
	void spawnEnemies(); // hanterar spawnScript

	static void OpenLuaLibs(lua_State* l);

private:
	Structure*** structures;
	lua_State* pathScript;
	lua_State* targetScript;
	lua_State* spawnScript;
};

