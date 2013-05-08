#ifndef D3DGRAPHICUTILITY_H
#define D3DGRAPHICUTILITY_H

#include <D3D11.h>
#include <D3DX11.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

//#include <D3D10_1.h>
//#include <D3D10.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment (lib,"dxerr.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "Effects11D.lib")
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "d3dx10.lib")
#endif

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }

struct FaceType
{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;

		FaceType()
		{
		}

};

enum PRIMITIVE_TOPOLOGIES
{
	TOPOLOGY_UNDEFINED		= 0,
	TOPOLOGY_POINTLIST		= 1,   
	TOPOLOGY_TRIANGLELIST	= 4,  
};

enum CPU_ACCESS_AND_MAP
{
	WRITE_DISCARD		= 4,
	WRITE_NO_OVERWRITE	= 5,
};

enum PASS_STATE
{
	PASS_GEOMETRY,
	PASS_LIGHT,
	PASS_FULLSCREENQUAD,
	PASS_HPBAR,
};



//enum BUFFER_TYPE
//{
//	VERTEX_BUFFER,
//	INDEX_BUFFER,
//	CONSTANT_BUFFER_VS,
//	CONSTANT_BUFFER_GS,
//	CONSTANT_BUFFER_PS,
//	BUFFER_TYPE_COUNT
//};
//
//enum BUFFER_USAGE
//{
//	BUFFER_DEFAULT,
//	BUFFER_STREAM_OUT_TARGET,
//	BUFFER_CPU_WRITE,
//	BUFFER_CPU_WRITE_DISCARD,
//	BUFFER_CPU_READ,
//	BUFFER_USAGE_COUNT
//};

struct BUFFER_INIT
{
	struct BUFFER_DESC
	{
		UINT        uByteWidth;
		D3D11_USAGE	eUsage;
		UINT        uBindFlags;
		UINT        uCPUAccessFlags;
		UINT        uMiscFlags;
		UINT        uStructureByteStride;

		BUFFER_DESC()
		{
			uByteWidth				= 0;
			eUsage					= D3D11_USAGE_DEFAULT;
			uBindFlags				= 0;
			uCPUAccessFlags			= 0;
			uMiscFlags				= 0;
			uStructureByteStride	= 0;
		}

		BUFFER_DESC(UINT byteWidth, D3D11_USAGE	usage, UINT bindFlags, UINT cpuAccessFlags, UINT miscFlags, UINT structureByteStride)
		{
			uByteWidth				= byteWidth;
			eUsage					= usage;
			uBindFlags				= bindFlags;
			uCPUAccessFlags			= cpuAccessFlags;
			uMiscFlags				= miscFlags;
			uStructureByteStride	= structureByteStride;
		}

	}desc;

	struct SUB_DATA
	{
		void*	pInitData;
		UINT	uSysMemPitch;
		UINT	uSysMemSlicePitch;

		SUB_DATA()
		{
			pInitData			= NULL;
			uSysMemPitch		= 0;
			uSysMemSlicePitch	= 0;
		}

		SUB_DATA(void* initData, UINT sysMemPitch, UINT sysMemSlicePitch)
		{
			pInitData			= initData;
			uSysMemPitch		= sysMemPitch;
			uSysMemSlicePitch	= sysMemSlicePitch;
		}

	}data;

	BUFFER_INIT()
	{
		desc;
		data;
	}
};


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