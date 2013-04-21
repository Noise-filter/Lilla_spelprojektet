#ifndef STDAFX_H
#define STDAFX_H

#include <windows.h>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include <d3dx11effect.h>
#include <d3dCompiler.h>
#include <iostream>

#include <string>
#include <ctime>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib,"dxerr.lib")

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
#define PI (3.14159265358979323846f)

struct MouseState
{
	int xPos;
	int yPos;
	WPARAM btnState;

	MouseState(int xPos, int yPos, WPARAM btnState)
	{
		this->xPos = xPos;
		this->yPos = yPos;
		this->btnState = btnState;
	}
	MouseState()
	{

	}
};

struct RenderData
{
	int meshID;
	int textureID;
	D3DXMATRIX worldMat;
	
	int lightID;
};

struct TowerUpgrade
{
	int hp;
	int attackSpeed;
	int dmg;
	int projectileSpeed;
	int range;

	TowerUpgrade()
	{
		this->hp = hp;
		this->attackSpeed = attackSpeed;
		this->dmg = dmg;
		this->projectileSpeed = projectileSpeed;
		this->range = range;
	}
	
	TowerUpgrade(int hp, int attackSpeed, int dmg, int projectileSpeed, int range)
	{
		this->hp = hp;
		this->attackSpeed = attackSpeed;
		this->dmg = dmg;
		this->projectileSpeed = projectileSpeed;
		this->range = range;
	}
};


enum STRUCTURE_TYPES
{
	TYPE_TOWER,
	TYPE_SUPPLY,
	TYPE_UPGRADE_HP,
	TYPE_UPGRADE_ATKSP,
	TYPE_UPGRADE_DMG,
	TYPE_UPGRADE_PRJSP,
	TYPE_UPGRADE_RANGE,
};
enum STRUCTURE_COSTS
{
	COST_TOWER = 20,
	COST_SUPPLY = 40,
	COST_UPGRADE = 60,
};
enum NODE_COLORS
{
	COLOR_RED,
	COLOR_GREEN,
};


const int screenWidth = 1024;
const int screenHeight = 768;

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "Effects11D.lib")
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "d3dx10.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// to find memory leaks
//////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
#define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
#define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define myDelete delete  // Set to dump leaks at the program exit.
#define myInitMemoryCheck() \
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define myDumpMemoryLeaks() \
	_CrtDumpMemoryLeaks()
#else
#define myMalloc malloc
#define myCalloc calloc
#define myRealloc realloc
#define myExpand _expand
#define myFree free
#define myMemSize _msize
#define myNew new
#define myDelete delete
#define myInitMemoryCheck()
#define myDumpMemoryLeaks()
#endif 
//////////////////////////////////////////////////////////////////////////


#endif