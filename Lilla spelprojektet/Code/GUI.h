#pragma once
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include "Statistics.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

enum BUTTONTYPE
{
	STARTGAME,
	SETTINGS,
	QUIT,
	PAUSED_CONTINUE,
	NEXT,
	LAST,
	BACK,
	MUTE,
	MAIN_MENU,
	RETRY
};

struct Text
{
	D3DXVECTOR2 pos;
	wchar_t* text;
	float textSize;
	UINT32 textColor;
	bool center;
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
		matrix._22 = scale.y;
		matrix._41 = pos.x;
		matrix._42 = pos.y;

		textureID = ID; //Fixa detta
	}
};

class GUI
{
public:
	GUI();
	~GUI();

	void render(Button*& btn, Text*& text);
	int update(MouseState *mouse, int& state, bool& muted, bool& retry);
	void createBtns(int state);
	int getNrOfBtns()const;
	int getNrOfText()const;
	GUI_Panel* getPanels()const;
	int getNrOfPanels()const;
	string getCurrentLevel()const;
	int getCurrentDiff()const;
	void setInGameText(int resource, int supply, int currPercent, int winPercent, int selectedStructure);

private:
	
	void clearBtns();
	void clearTexts();
	void clearPanels();
	bool checkBtn(MouseState *mousePos, Button btn);
	int changeState(Button btn);
	Button createBtn(D3DXVECTOR2 pos, BUTTONTYPE type);
	Text createTextBox(D3DXVECTOR2 pos, wchar_t* text, float size, UINT32 color);
	void createPanels(int state);
	void createLevelList();
	void changeText(D3DXVECTOR2 pos, BUTTONTYPE type);
	void initDifficulty();
	wstring convertStrToWstr(string text);
	string convertWstrToStr(wstring text);
	void getEndStats();
	string convertFloat(float value);
	void setLeftAligned(Text& text);

	Button* menuBtns;
	int nrOfBtns;
	Text* textBoxes;
	int nrOfBoxes;
	GUI_Panel* panels;
	int nrOfPanles;
	wstring wStats[10];

	int GUI_STATE;
	int old_GUI_STATE;
	int lastPlayingState;
	float midScreenW;
	float midScreenH;
	bool muted;
	bool retry;
	bool first;
	int currentBuilding;

	wstring* levelList;
	int nrOfLevels;
	int currentLevel;

	wstring* difficultyList;
	int nrOfDifficultys;
	int currentDifficulty;
};