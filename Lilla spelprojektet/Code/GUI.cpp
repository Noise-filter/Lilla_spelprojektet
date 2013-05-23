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
	retry = false;
	first = true;
	createLevelList();
	this->currentLevel = 0;
	initDifficulty();
	createBtns(STATE_MENU);
	createPanels(STATE_MENU);
	endStats = Statistics::Getinstance();
	
}

GUI::~GUI()
{

	clearBtns();
	clearTexts();
	clearPanels();
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

int GUI::update(MouseState *mouse, int& state, bool& muted, bool& retry)
{
	if(GUI_STATE != state)
	{
		old_GUI_STATE = GUI_STATE;
		if(state == STATE_PAUSED && (old_GUI_STATE == STATE_GAMESTART || old_GUI_STATE == STATE_PLAYING))
		{
			lastPlayingState = GUI_STATE;
		}
		if(state != STATE_GAMESTART && state != STATE_PLAYING && old_GUI_STATE != STATE_PAUSED && old_GUI_STATE != STATE_WIN && old_GUI_STATE != STATE_LOSE)
		{
			clearTexts();
		}
		clearBtns();
		clearPanels();
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
	retry = this->retry;
	return state;
}

void GUI::createBtns(int state)
{
	
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
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 140), L"Caecus", 62, 0x800000ff);
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
		if(first)
		{
		this->nrOfBoxes+=3;
		first = false;
		}
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		//this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.5*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Information:", 18, 0xffffffff);
		this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.8*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Resourse:", 18, 0xffffffff);
		this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5*(0.775*SCREEN_WIDTH),0.5*(1.875*SCREEN_HEIGHT)), L"Supply:", 18, 0xffffffff);
		this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5*(1.2*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Time:", 18, 0xffffffff);
	}
	else if(state == STATE_WIN)
	{
		
		getEndStats();
		this->nrOfBoxes+=1;
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		setLeftAligned();
		this->textBoxes[10] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 150), L"You Won!", 62, 0x800000ff);
		this->nrOfBtns =2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW - 100, midScreenH + 100), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW + 100, midScreenH + 100), RETRY);
	}
	else if(state == STATE_LOSE)
	{
		getEndStats();
		this->nrOfBoxes+=1;
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		setLeftAligned();
		this->textBoxes[10] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 150), L"You Lost", 62, 0x800000ff);
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW - 100, midScreenH + 100), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW + 100, midScreenH + 100), RETRY);
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

void GUI::clearBtns()
{
	SAFE_DELETE_ARRAY(menuBtns);
	this->nrOfBtns = 0;
}

void GUI::clearTexts()
{
	SAFE_DELETE_ARRAY(textBoxes);
	this->nrOfBoxes = 0;
}

void GUI::clearPanels()
{
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
	text.center = true;
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
	if(type == RETRY)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Retry";
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
	temp.center = true;
	return temp;
}

void GUI::createPanels(int state)
{
	if(state == STATE_MENU || state == SETTINGS || state == STATE_NEWGAME || state == STATE_PAUSED || state == STATE_WIN || state == STATE_LOSE)
	{
		this->nrOfPanles = 1;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), TEXTURE_MENU);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING )
	{
		this->nrOfPanles = 1;
		this->panels = new GUI_Panel[nrOfPanles];
		//this->panels[0] = GUI_Panel(D3DXVECTOR2(-0.8, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_BUILDING);
		//this->panels[1] = GUI_Panel(D3DXVECTOR2(-0.4, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_INFO);
		//this->panels[2] = GUI_Panel(D3DXVECTOR2(0.0, -0.8), D3DXVECTOR2(0.2, 0.2), TEXTURE_STATS);
		//this->panels[3] = GUI_Panel(D3DXVECTOR2(0.6, -0.7), D3DXVECTOR2(0.4, 0.1), TEXTURE_TIME_GOAL);
		//this->panels[4] = GUI_Panel(D3DXVECTOR2(0.6, -0.9), D3DXVECTOR2(0.4, 0.1), TEXTURE_BUILDINGS_HOTKEY);
		this->panels[0] = GUI_Panel(D3DXVECTOR2(0, -0.8), D3DXVECTOR2(1, 0.2), TEXTURE_FRAME);
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
		state = lastPlayingState;
		first = true;
	}
	else if(btn.type == BACK)
	{
		state = old_GUI_STATE;
	}
	else if(btn.type == MAIN_MENU)
	{
		 state = STATE_MENU;
		 first = true;
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
	else if(btn.type == RETRY)
	{
		state = STATE_GAMESTART;
		this->retry = true;
		first = true;
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

	//läs från fil istället

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

void GUI::getEndStats()
{
	int nrOfStats = 10;
	this->nrOfBoxes = nrOfStats;
	this->textBoxes = new Text[this->nrOfBoxes];
	char temp[255];
	string Stats[10];
	Stats[0] = "Level: " + getCurrentLevel();
	Stats[1] = "Score: ";
	Stats[1].append( itoa(this->endStats->totalScore, temp, 10) );
	Stats[2] = "Resources: ";
	Stats[2].append( itoa(this->endStats->totalRes, temp, 10) );
	Stats[3] = "Supplys: ";
	Stats[3].append (itoa(this->endStats->totalSupply, temp, 10) );
	Stats[4] = "Time: ";
	Stats[4].append( itoa(this->endStats->totalTime, temp, 10) );
	Stats[5] = "Building built: ";
	Stats[5].append( itoa(this->endStats->totalNrOfBuildings, temp, 10) );
	Stats[6] = "Enemies killed: ";
	Stats[6].append( itoa(this->endStats->totalEnemiesKilled, temp, 10) );
	Stats[7] = "Nr of Upgrades: ";
	Stats[7].append( itoa(this->endStats->totalNrOfUpgrades, temp, 10) );
	Stats[8] = "Buildings at max lv: ";
	Stats[8].append( itoa(this->endStats->totalNrOfMaxLvlTowers, temp, 10) );
	Stats[9] = "Average lv: ";
	Stats[9].append( convertFloat(this->endStats->averageTowerLvl) );

	
	for(int i = 0; i < nrOfStats; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.65*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.7*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.75*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.8*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.85*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);
	this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.9*SCREEN_HEIGHT)), (wchar_t*)wStats[5].c_str(), 18, 0xffffffff);
	this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(0.95*SCREEN_HEIGHT)), (wchar_t*)wStats[6].c_str(), 18, 0xffffffff);
	this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(1*SCREEN_HEIGHT)), (wchar_t*)wStats[7].c_str(), 18, 0xffffffff);
	this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(1.05*SCREEN_HEIGHT)), (wchar_t*)wStats[8].c_str(), 18, 0xffffffff);
	this->textBoxes[9] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(1.1*SCREEN_HEIGHT)), (wchar_t*)wStats[9].c_str(), 18, 0xffffffff);

}

string GUI::convertFloat(float value) 
{
	std::ostringstream o;
	if (!(o << value))
		return "";
  return o.str();
}

void GUI::setLeftAligned()
{
	for(int i = 0; i<this->nrOfBoxes; i++)
	{
		this->textBoxes[i].center = false;
	}
}

void GUI::setInGameText(int resource, int supply, float currPercent, float winPercent)
{
	
	if(first)
	{
		this->nrOfBoxes = 5;
		this->textBoxes = new Text[this->nrOfBoxes];
		
	}
	char temp[255];
	string Stats[5];
	Stats[0] = itoa(resource, temp, 10);
	Stats[1] = itoa(supply, temp, 10);
	Stats[2] = itoa(this->endStats->totalTime, temp, 10);
	Stats[3] = convertFloat(currPercent);
	Stats[4] = convertFloat(winPercent);
	

	for(int i = 0; i < 5; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.875*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5*(1.3*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5*(1.7*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5*(1.8*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);

}