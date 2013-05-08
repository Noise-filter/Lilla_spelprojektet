#pragma once
#include "TextObject.h"
#include "Buffer.h"
#include "Shader.h"
#include <string>

using namespace std;

enum BUTTONTYPE
{
	STARTGAME,
	SETTINGS,
	QUIT,
	PAUSED_CONTINUE
};

struct Button
	{
		D3DXVECTOR2 pos;
		D3DXVECTOR2 size;
		wchar_t* text;
		BUTTONTYPE type;
		UINT32 textColor;
	};

class GUI
{
public:
	GUI();
	~GUI();

	void render(ID3D11Device *pDevice, ID3D11DeviceContext *pContext);
	int update(MouseState *mouse, GAMESTATES state);
	void createBtns(GAMESTATES state);
	void clearBtns();
	

private:
	
	
	bool checkBtn(MouseState *mousePos, Button btn);
	GAMESTATES changeState(Button btn);
	Button createBtn(D3DXVECTOR2 pos, BUTTONTYPE type);

	Button* menuBtns;
	int nrOfBtns;
	TextObject* textObjects;
	GAMESTATES GUI_STATE;

};