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
	LAST
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
	

private:
	
	void clear();
	bool checkBtn(MouseState *mousePos, Button btn);
	int changeState(Button btn);
	Button createBtn(D3DXVECTOR2 pos, BUTTONTYPE type);
	Text createTextBox(D3DXVECTOR2 pos, wchar_t* text, float size, UINT32 color);

	Button* menuBtns;
	int nrOfBtns;
	Text* textBoxes;
	int nrOfBoxes;

	int GUI_STATE;

};