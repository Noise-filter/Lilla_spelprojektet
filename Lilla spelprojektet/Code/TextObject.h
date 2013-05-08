#pragma once

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include <string>

using namespace std;



class TextObject
{
public:
	TextObject();
	TextObject(wchar_t* text, float size, D3DXVECTOR2 pos, UINT32 color);
	~TextObject();
	void drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext);

private:
	wchar_t* text;
	float size;
	D3DXVECTOR2 pos;
	UINT32 color;

};