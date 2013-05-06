#pragma once
#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
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
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		string text;
		BUTTONTYPE type;
	};

class GUI
{
public:
	GUI(void);
	~GUI(void);

	void render();
	int update(MouseState *mouse, GAMESTATES state);
	void createBtns(GAMESTATES state);
	void clearBtns();
	

private:
	
	
	bool checkBtn(MouseState *mousePos, Button btn);
	GAMESTATES changeState(Button btn);
	Button createBtn(D3DXVECTOR3 pos, BUTTONTYPE type);

	Button* menuBtns;
	int nrOfBtns;
	GAMESTATES GUI_STATE;

};