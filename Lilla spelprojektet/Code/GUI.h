#pragma once
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include <string>

using namespace std;

enum BUTTONTYPE
{
	STARTGAME,
	SETTINGS,
	QUIT,
	PAUSED_CONTINUE,
	MENU,
	NEXT,
	LAST,
	MAIN_MENU
};

struct Text
{
	D3DXVECTOR2 pos;
	wchar_t* text;
	float textSize;
	UINT32 textColor;
};

struct Button
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	BUTTONTYPE type;
	Text text;
};

struct GUI_Panel
{
	D3DXMATRIX matrix;
	int textureID;
	GUI_Panel()
	{

	}
	GUI_Panel(D3DXVECTOR2 pos, D3DXVECTOR2 scale, int ID)
	{
		D3DXMatrixIdentity(&matrix);
		matrix._11 = scale.x;
		matrix._33 = scale.y;
		matrix._41 = pos.x;
		matrix._43 = pos.y;

		textureID = ID;
	}
};

class GUI
{
public:
	GUI();
	~GUI();

	void render(Button*& btn, Text*& text);
	int update(MouseState *mouse, int& state);
	void createBtns(int state);
	int getNrOfBtns()const;
	int getNrOfText()const;
	GUI_Panel* getPanels()const;
	int getNrOfPanels()const;
	

private:
	
	void clear();
	bool checkBtn(MouseState *mousePos, Button btn);
	int changeState(Button btn);
	Button createBtn(D3DXVECTOR2 pos, BUTTONTYPE type);
	Text createTextBox(D3DXVECTOR2 pos, wchar_t* text, float size, UINT32 color);
	void createPanels(int state);


	Button* menuBtns;
	int nrOfBtns;
	Text* textBoxes;
	int nrOfBoxes;
	GUI_Panel* panels;
	int nrOfPanles;

	int GUI_STATE;
	float midScreenW;
	float midScreenH;
};