#include "TextObject.h"

TextObject::TextObject()
{

}

TextObject::TextObject(wchar_t* text, float size, D3DXVECTOR2 pos, UINT32 color)
{
	this->text = text;
	this->size = size;
	this->pos = pos;
	this->color = color;
}

TextObject::~TextObject()
{

}

void TextObject::drawText(ID3D11Device *pDevice, ID3D11DeviceContext *pContext)
{
	


	

}