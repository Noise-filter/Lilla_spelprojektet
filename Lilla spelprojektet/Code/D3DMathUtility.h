#ifndef D3DMATHUTILITY_H
#define D3DMATHUTILITY_H
#include <D3D10_1.h>
#include <D3DX10math.h>

#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3
#define Vec4 D3DXVECTOR4
#define Matrix D3DXMATRIX

#define PI (3.14159265358979323846f)

#define NROFDIFFERENTMESHES (12)

struct GameSettings
{
	int enemiesPerMin;
	int ScreenHeight;
	int ScreenWidth;
	int mapSize;
	int quadSize;

	//add more if needed
}

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
enum ENTITY_FLAGS
{
	ENTITY_MAINBUILDING,
	ENTITY_SUPPLY,
	ENTITY_TOWERBASE,
	ENTITY_TOWERTOP,
	ENTITY_NODE,
	ENTITY_ENEMY,
	ENTITY_PROJECTILE,
	ENTITY_UPGRADE_HP,
	ENTITY_UPGRADE_ATKSP,
	ENTITY_UPGRADE_DMG,
	ENTITY_UPGRADE_PRJSP,
	ENTITY_UPGRADE_RANGE,
	ENTITY_GUI,
};

enum GAMESTATES
{
   STATE_MENU,
   STATE_SETTINGS,
   STATE_GAMESTART,
   STATE_PLAYING,
   STATE_PAUSED, 
   STATE_WIN,
   STATE_LOSE
};

enum BUILDABLE_ENTITY_FLAGS
{
	BUILDABLE_MAINBUILDING,
	BUILDABLE_SUPPLY,
	BUILDABLE_TOWER,
	BUILDABLE_UPGRADE_HP,
	BUILDABLE_UPGRADE_ATKSP,
	BUILDABLE_UPGRADE_DMG,
	BUILDABLE_UPGRADE_PRJSP,
	BUILDABLE_UPGRADE_RANGE,
};

enum TEXTURE_FLAGS
{
	TEXTURE_MAINBUILDING1,
	TEXTURE_SUPPLY1,
	TEXTURE_TOWER1,
	TEXTURE_ENEMY1,
	TEXTURE_NODE1,
	TEXTURE_PARTICLESYSTEM1,
	TEXTURE_GAMEFIELD1,
	TEXTURE_GUI1,
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

	UpgradeStats(int id ,int hp, int dmg, int atkSpeed, int prjSpeed, int range)
	{
		this->id = id;
		this->hp = hp;
		this->dmg = dmg;
		this->atkSpeed = atkSpeed;
		this->prjSpeed = prjSpeed;
		this->range = range;
	}	

	int id;
	int hp;
	int dmg;
	int atkSpeed;
	int prjSpeed;
	int range;
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



#endif