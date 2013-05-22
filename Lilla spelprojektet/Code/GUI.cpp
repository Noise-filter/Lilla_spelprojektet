#include "GUI.h"

GUI::GUI()
{
	this->menuBtns = NULL;
	this->textBoxes = NULL;
	this->panels = NULL; 
	this->nrOfBtns = 0;
	this->nrOfBoxes = 0;
	this->nrOfPanles = 0;
	this->GUI_STATE = 0;
	midScreenW = (float)SCREEN_WIDTH/2;
	midScreenH = (float)SCREEN_HEIGHT/2;
	muted = false;
	createLevelList();
	this->currentLevel = 0;
	initDifficulty();
	createBtns(STATE_MENU);
	createPanels(STATE_MENU);
}

GUI::~GUI()
{

	clear();
	SAFE_DELETE_ARRAY(levelList);
	this->nrOfLevels = 0;
	SAFE_DELETE_ARRAY(difficultyList);
	this->nrOfDifficultys = 0;

}


void GUI::render(Button*& btns, Text*& text)
{
	
		btns = this->menuBtns;
		text = this->textBoxes;
		
	
}

int GUI::update(MouseState *mouse, int& state, bool& muted)
{
	if(GUI_STATE != state)
	{
		old_GUI_STATE = GUI_STATE;
		clear();
		createBtns(state);
		createPanels(state);
		GUI_STATE = state;
	}

	for(int i = 0; i < this->nrOfBtns; i++)
	{
		bool check = checkBtn(mouse, this->menuBtns[i]);
		if(check)
		{
			state = changeState(this->menuBtns[i]);
			
		}
	}
	muted = this->muted;
	return state;
}

void GUI::createBtns(int state)
{

	//if(this->nrOfBtns != 0 || this->nrOfBoxes != 0)
	{
		//clear();
	}
	wchar_t* level = (wchar_t*)levelList[currentLevel].c_str();
	wchar_t* difficulty = (wchar_t*)difficultyList[currentDifficulty].c_str();

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH - 30), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 10), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 50), QUIT);
		this->nrOfBoxes = 1;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 140), L".exe", 62, 0x800000ff);
	}
	else if(state == STATE_SETTINGS)
	{
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH - 30), MUTE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 120), BACK);
	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 30), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + +60), MAIN_MENU);
	}
	else if(state == STATE_NEWGAME)
	{
		this->nrOfBoxes = 2;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 100), level, 36, 0x800000ff);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 50), difficulty, 36, 0x800000ff);
		this->nrOfBtns = 6;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x - 80, textBoxes[0].pos.y), LAST);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x + 80, textBoxes[0].pos.y), NEXT);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x - 80, textBoxes[1].pos.y), LAST);
		this->menuBtns[3] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x + 80, textBoxes[1].pos.y), NEXT);
		this->menuBtns[4] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 75), STARTGAME);
		this->menuBtns[5] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 120), BACK);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING)
	{
		this->nrOfBoxes = 4;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(0.6*SCREEN_WIDTH),0.5*(1.65*SCREEN_HEIGHT)), L"Information:", 20, 0xff000000);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(0.95*SCREEN_WIDTH),0.5*(1.65*SCREEN_HEIGHT)), L"Resourse:", 20, 0xff000000);
		this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5*(0.95*SCREEN_WIDTH),0.5*(1.825*SCREEN_HEIGHT)), L"Supply:", 20, 0xff000000);
		this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5*(1.275*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Time:", 20, 0xff000000);
	}
	else if(state == STATE_WIN)
	{
		this->nrOfBoxes = 1;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 100), L"You Won!", 62, 0x800000ff);
		this->nrOfBtns = 1;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW - 100, midScreenH + 100), MAIN_MENU);
	}
	else if(state == STATE_LOSE)
	{
		this->nrOfBoxes = 1;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 100), L"You Lost", 62, 0x800000ff);
		this->nrOfBtns = 1;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW - 100, midScreenH + 100), MAIN_MENU);
	}
}

int GUI::getNrOfBtns()const
{
	return this->nrOfBtns;
}

int GUI::getNrOfText()const
{
	return this->nrOfBoxes;
}

GUI_Panel* GUI::getPanels()const
{
	return this->panels;
}

int GUI::getNrOfPanels()const
{
	return this->nrOfPanles;
}

void GUI::clear()
{
	SAFE_DELETE_ARRAY(menuBtns);
	this->nrOfBtns = 0;

	SAFE_DELETE_ARRAY(textBoxes);
	this->nrOfBoxes = 0;

	SAFE_DELETE_ARRAY(panels);
	this->nrOfPanles = 0;
}

Button GUI::createBtn(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	Button btn;
	Text text;
	
	btn.pos = pos;
	btn.type = type;
	text.pos = pos;
	text.textSize = 36;
	//text.pos.y = pos.y;
	text.textColor = 0x800000ff;

	if(type == STARTGAME)
	{
		btn.size = D3DXVECTOR2(100, 15);
		text.text = L"New Game";
	}
	if(type == SETTINGS)
	{
		btn.size = D3DXVECTOR2(100, 18);
		text.text = L"Settings";
	}
	if(type == QUIT)
	{
		btn.size = D3DXVECTOR2(80, 18);
		text.text = L"Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		btn.size = D3DXVECTOR2(100, 20);
		text.text = L"CONTINUE";
	}
	if(type == NEXT)
	{
		btn.size = D3DXVECTOR2(20, 20);
		text.text = L">";
	}
	if(type == LAST)
	{
		btn.size = D3DXVECTOR2(20, 20);
		text.text = L"<";
	}
	if(type == BACK)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Back";
	}
	if(type == MUTE)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Mute";
	}
	if(type == MAIN_MENU)
	{
		btn.size = D3DXVECTOR2(100, 20);
		text.text = L"Main Menu";
	}

	btn.text = text;

	return btn;
}

Text GUI::createTextBox(D3DXVECTOR2 pos, wchar_t* text, float size, UINT32 color)
{
	Text temp;
	temp.pos = pos;
	temp.text = text;
	temp.textSize = size;
	temp.textColor = color;
	return temp;
}

void GUI::createPanels(int state)
{
	if(state == STATE_MENU || state == SETTINGS || state == STATE_NEWGAME || state == STATE_PAUSED)
	{
		this->nrOfPanles = 1;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), TEXTURE_MENU);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING )
	{
		this->nrOfPanles = 5;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(-0.8, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_BUILDING);
		this->panels[1] = GUI_Panel(D3DXVECTOR2(-0.4, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_INFO);
		this->panels[2] = GUI_Panel(D3DXVECTOR2(0.0, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_STATS);
		this->panels[3] = GUI_Panel(D3DXVECTOR2(0.6, -0.7), D3DXVECTOR2(0.4, 0.1), TEXTURE_TIME_GOAL);
		this->panels[4] = GUI_Panel(D3DXVECTOR2(0.6, -0.9), D3DXVECTOR2(0.4, 0.1), TEXTURE_BUILDINGS_HOTKEY);
	}
	else if(state == STATE_WIN || state == STATE_LOSE)
	{

	}
}

bool GUI::checkBtn(MouseState *mousePos, Button btn)
{
	if(mousePos->btnState == VK_LBUTTON)
	{
		if(mousePos->xPos > btn.pos.x - btn.size.x)
		{
			if(mousePos->xPos < btn.pos.x + btn.size.x)
			{
				if(mousePos->yPos < btn.pos.y + btn.size.y)
				{
					if(mousePos->yPos > btn.pos.y - btn.size.y)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

int GUI::changeState(Button btn)
{
	int state = GUI_STATE;
	if(btn.type == STARTGAME && state == STATE_MENU)
	{
		state = STATE_NEWGAME;
	}
	else if(btn.type == STARTGAME && state == STATE_NEWGAME)
	{
		state = STATE_GAMESTART;
	}
	else if(btn.type == SETTINGS)
	{
		state = STATE_SETTINGS;
	}
	else if(btn.type == PAUSED_CONTINUE)
	{
		state = old_GUI_STATE;
	}
	else if(btn.type == BACK)
	{
		state = old_GUI_STATE;
	}
	else if(btn.type == MAIN_MENU)
	{
		 state = STATE_MENU;
	}
	else if(btn.type == QUIT)
	{
		state = STATE_QUIT;
	}
	else if(btn.type == MUTE)
	{
		muted = !muted;
	}
	else if(btn.type == NEXT || btn.type == LAST)
	{
		changeText(btn.pos, btn.type);
	}
	return state;
}

void GUI::createLevelList()
{
	ifstream fin;
	fin.open("Level list.txt");
	string attribute;
	int value;

	if(fin.fail() == true)
	{
		cout << "FAILED TO READ LEVELS FROM FILE" << endl;
	}

	fin >> value;
	this->nrOfLevels = value;
	this->levelList = new wstring[nrOfLevels];
	int count = 0;
	while(count != nrOfLevels)
	{
		fin >> attribute;
		for(int i = 0; i < (int)attribute.size(); i++)
		{
			this->levelList[count].push_back(attribute[i]);
		}
		//this->levelList[count] = wstring(attribute.begin(), attribute.end());

		count++;
	}

	//l�s fr�n fil ist�llet

	fin.close();
}

void GUI::changeText(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	if(pos.y == textBoxes[0].pos.y && type == NEXT)
	{
		this->currentLevel = (currentLevel+1) % nrOfLevels;
	}
	else if(pos.y == textBoxes[0].pos.y && type == LAST)
	{
		this->currentLevel--;
		if(this->currentLevel < 0)
		{
			this->currentLevel = this->nrOfLevels-1;
		}
	}

	textBoxes[0].text = (wchar_t*)levelList[currentLevel].c_str();

	if(pos.y == textBoxes[1].pos.y && type == NEXT)
	{
		this->currentDifficulty = (currentDifficulty+1) % nrOfDifficultys;
	}
	else if(pos.y == textBoxes[1].pos.y && type == LAST)
	{
		this->currentDifficulty--;
		if(this->currentDifficulty < 0)
		{
			this->currentDifficulty = this->nrOfDifficultys-1;
		}
	}
	wchar_t* difficulty = (wchar_t*)difficultyList[currentDifficulty].c_str();
	textBoxes[1].text = difficulty;
}

string GUI::getCurrentLevel()const
{
	string temp;
	for(int i = 0; i < (int)levelList[currentLevel].size(); i++)
	{
		temp.push_back(levelList[currentLevel][i]);
	}

	return temp;
}

void GUI::initDifficulty()
{
	this->nrOfDifficultys = 3;
	this->difficultyList = new wstring[nrOfDifficultys];
	this->difficultyList[0] = L"Easy";
	this->difficultyList[1] = L"Medium";
	this->difficultyList[2] = L"Hard";
	this->currentDifficulty = 0;
}

int GUI::getCurrentDiff()const
{
	return this->currentDifficulty;
}

wstring GUI::convertStrToWstr(string text)
{
	wstring temp(text.begin(), text.end());
	//copy(text.begin(), text.end(), temp.begin());
	return temp;
}

string GUI::convertWstrToStr(wstring text)
{
	string temp(text.begin(), text.end());
    //copy(text.begin(), text.end(), temp.begin());
    return temp;
}