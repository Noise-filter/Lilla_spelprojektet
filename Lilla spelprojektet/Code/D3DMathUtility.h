#ifndef D3DMATHUTILITY_H
#define D3DMATHUTILITY_H
#include <D3D10_1.h>
#include <D3DX10math.h>

#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3
#define Vec4 D3DXVECTOR4
#define Matrix D3DXMATRIX

#define PI (3.14159f)

#define NROFDIFFERENTMESHES (15)

struct GameSettings
{
	int resCD;
	int enemiesPerMin;
	int resPerTick;
	int difficulty; //send to the script
	//add more if needed
};
enum DIFFICULTIES
{
	DIFF_EASY,
	DIFF_MEDIUM,
	DIFF_HARD
};

struct MESH_P
{
	Vec3 pos;
	float pad;
	MESH_P()
	{
		this->pos = Vec3(0.0f, 0.0f, 0.0f);
		pad = 0;
	};

	MESH_P(Vec3 pos)
	{
		this->pos = pos;
		pad = 9;
	};
};

struct MESH_PNC
{
	Vec3 pos;
	Vec3 normal;
	Vec3 color;

	MESH_PNC()
	{
		this->pos    = Vec3(0.0f, 0.0f, 0.0f);
		this->normal = Vec3(0.0f, 0.0f, 0.0f);
		this->color  = Vec3(0.0f, 0.0f, 0.0f);
	};

	MESH_PNC(Vec3 pos, Vec3 normal , Vec3 color)
	{
		this->pos    = pos;
		this->normal = normal;
		this->color  = color;
	};
};

struct MESH_PUV
{
	Vec3 pos;
	Vec2 uv;

	MESH_PUV()
	{
		this->pos = Vec3(0.0f, 0.0f, 0.0f);
		this->uv	  = Vec2(0.0f, 0.0f);
	};

	MESH_PUV(Vec3 pos, Vec2 uv)
	{
		this->pos	= pos;
		this->uv		= uv;
	};
};

struct MESH_PNUV
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;

	MESH_PNUV()
	{
		this->pos		= Vec3(0.0f, 0.0f, 0.0f);
		this->normal		= Vec3(0.0f, 0.0f, 0.0f);
		this->uv			= Vec2(0.0f, 0.0f);
	};
	
	MESH_PNUV(Vec3 pos, Vec3 normal, Vec2 uv)
	{
		this->pos		= pos;
		this->normal		= normal;
		this->uv			= uv;
	};
};

enum ENTITY_FLAGS
{
	ENTITY_MAINBUILDING,
	ENTITY_SUPPLYBASE,
	ENTITY_TOWERBASE,
	ENTITY_UPGRADE_OFFENSE,
	ENTITY_UPGRADE_DEFENSE,
	ENTITY_UPGRADE_RES,
	ENTITY_SUPPLYTOP,
	ENTITY_TOWERTOP,
	ENTITY_NODE_GREEN,
	ENTITY_ENEMY,
	ENTITY_PROJECTILE,
	ENTITY_PLANE,
	ENTITY_SPIRALSPHERE,
	ENTITY_NEUTRALGATE,
	ENTITY_PYRAMID,
	ENTITY_GUI,
};

enum GAMESTATES
{
   STATE_MENU,
   STATE_SETTINGS,
   STATE_NEWGAME,
   STATE_LOADING,
   STATE_GAMESTART,
   STATE_PLAYING,
   STATE_PAUSED, 
   STATE_WIN,
   STATE_LOSE,
   STATE_QUIT
};


enum BUILDABLE_ENTITY_FLAGS
{
	BUILDABLE_MAINBUILDING,
	BUILDABLE_SUPPLY,
	BUILDABLE_TOWER,
	BUILDABLE_UPGRADE_OFFENSE,
	BUILDABLE_UPGRADE_DEFENSE,
	BUILDABLE_UPGRADE_RES,
};

enum TEXTURE_FLAGS
{
	
	TEXTURE_MENU,
	TEXTURE_MAINBUILDING,
	TEXTURE_SUPPLY,
	TEXTURE_SUPPLY_TINY,
	TEXTURE_TOWER,
	TEXTURE_TOWER_TINY,
	TEXTURE_ATK,
	TEXTURE_ATK_TINY,
	TEXTURE_DEF,
	TEXTURE_DEF_TINY,
	TEXTURE_MONEY,
	TEXTURE_MONEY_TINY,
	TEXTURE_FRAME,
	
	
};

enum LIGHT_FLAGS
{
	LIGHT_NONE			= -1,
	LIGHT_DIRECTIONAL	= 0,
	LIGHT_POINT			= 1,
	LIGHT_SPOT			= 2,
};

enum STRUCTURE_COSTS
{
	COST_TOWER = 20,
	COST_SUPPLY = 40,
	COST_UPGRADE = 60,
};

enum STRUCTURE_HP
{
	HP_MAIN = 500,
	HP_TOWER = 100,
	HP_SUPPLY = 100,
	HP_UPGRADE = 100
};

enum NODE_COLORS
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_GREY,
};

struct UpgradeStats
{
	UpgradeStats()
	{
	}

	UpgradeStats(int id, float hp, float dmg, float atkSpeed, float range)
	{
		this->id = id;
		this->hp = hp;
		this->dmg = dmg;
		this->atkSpeed = atkSpeed;
		this->range = range;
	}	

	int id;
	float hp;
	float dmg;
	float atkSpeed;
	float range;
};

struct RenderData
{
	int meshID;
	int textureID;
	Matrix worldMat;
	int lightID;

	RenderData()
	{
		meshID		= 0;
		textureID	= 0;
		worldMat	= Matrix(0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f);
		lightID		= 0;
	};

	RenderData(int meshID, int textureID, Matrix world, int lightID)
	{
		this->meshID 	= meshID;
		this->textureID = textureID;
		this->worldMat	= world;
		this->lightID	= lightID;
	};
};

struct HPBarInfo
{
	float hpPercent;
	Matrix translate;

	HPBarInfo()
	{
		D3DXMatrixIdentity(&translate);
		hpPercent = 0;

	}

	HPBarInfo(Matrix trans, float hpPercent)
	{
		translate = trans;
		this->hpPercent = hpPercent;
	}
};

struct INSTANCEDATA
{
	Matrix mWorld;
	UINT iTextureID;

	INSTANCEDATA()
	{
		iTextureID	= 0;
		mWorld		= Matrix(0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f);
	};

	INSTANCEDATA(Matrix world, UINT ID)
	{
		iTextureID	= ID;
		mWorld		= world;
	};
};

struct MatrixInstance
{
	Matrix world;
};

struct POINTLIGHTINSTANCE
{
	Vec3 vLightPosition;
	Vec3 vLightColor;
	float fLightRadius;
	
	POINTLIGHTINSTANCE()
	{
		vLightPosition = Vec3(0.0f , 0.0f , 0.0f);
		vLightColor    = Vec3(0.0f , 0.0f , 0.0f);
		fLightRadius   = 0;
	};

	POINTLIGHTINSTANCE(Vec3 position , Vec3 color, float radius)
	{
		vLightPosition = position;
		vLightColor    = color;
		fLightRadius   = radius;
	};

};


#endif