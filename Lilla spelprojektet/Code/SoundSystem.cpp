#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
	system = NULL;

	result = FMOD_OK;
	lastError = FMOD_OK;

	this->nrOfSounds = 0;

}

SoundSystem::~SoundSystem()
{
	//delete m_SoundSystem;
}

SoundSystem* SoundSystem::m_SoundSystem = NULL;
SoundSystem* SoundSystem::Getinstance()
{
	if(m_SoundSystem==NULL)
	{
		m_SoundSystem = new SoundSystem();
	}
	return m_SoundSystem;
}

bool SoundSystem::init()
{
	//create system
	result = FMOD::System_Create(&system);
	if(errorCheck(result))
		return false;

	unsigned int version;
	//Check for old version
	result = system->getVersion(&version);
	if(errorCheck(result))
		return false;

	if(version < FMOD_VERSION)
	{
		char text[256];
		sprintf_s(text, sizeof(text), "Error! You are using an old version of FMOD %08x. This program requires %08x", version, FMOD_VERSION);
		MessageBox(NULL, text, "Error", MB_OK);
	}

	int numDrivers;
	//check number of drivers
	result = system->getNumDrivers(&numDrivers);
	if(errorCheck(result))
		return false;

	if(numDrivers == 0)	//no sound
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		if(errorCheck(result))
			return false;
	}
	else
	{
		FMOD_SPEAKERMODE speakerMode;
		FMOD_CAPS caps;

		result = system->getDriverCaps(0, &caps, 0, &speakerMode);
		if(errorCheck(result))
			return false;

		//set user selected speaker mode
		result = system->setSpeakerMode(speakerMode);
		if(errorCheck(result))
			return false;

		if(caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			//user has the 'Acceleration' slider set to off! This is really bad for latency!
			result = system->setDSPBufferSize(1024, 10);
			if(errorCheck(result))
				return false;
		}

		char name[256];
		result = system->getDriverInfo(0, name, 256, 0);
		if(errorCheck(result))
			return false;

		if(strstr(name, "SigmaTel"))
		{
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			if(errorCheck(result))
				return false;
		}
	}

	//init system
	result = system->init(10, FMOD_INIT_NORMAL, 0);
	if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if(errorCheck(result))
			return false;

		result = system->init(10, FMOD_INIT_NORMAL, 0);
	}

	if(errorCheck(result))
		return false;

	//get master channel group
	result = system->getMasterChannelGroup(&masterChannelGroup);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::shutdown()
{
	//release all sounds
	for(int i = 0; i < (int)sounds.size(); i++)
	{
		if(sounds.at(i))
		{
			sounds.at(i)->sound->release();
			if(errorCheck(result))
				return false;
			delete sounds.at(i);
		}
	}

	//release system
	if(system)
	{
		result = system->release();
		if(errorCheck(result))
			return false;
	}

	delete this->m_SoundSystem;

	return true;
}

bool SoundSystem::update()
{


	result = system->update();
	if(errorCheck(result))
		return false;

	if(!isplaying(playlist))
	{
	
		playNextSong(playlist);
	}


	FMOD::Memory_GetStats(&memoryUsage, &maxMemoryUsage);

	return true;
}

//will not create multiple streams of same file
Sound* SoundSystem::createStream(const char* filename)
{
	Sound* sound = new Sound();
	result = system->createStream(filename, FMOD_DEFAULT, 0, &sound->sound);
	errorCheck(result);

	char name[255];
	char name2[255];
	bool found = false;
	int i = 0;
	for(i = 0; i < (int)sounds.size(); i++)	//check to se if the sound is already created
	{
		sounds.at(i)->sound->getName(name, 255);
		sound->sound->getName(name2, 255);

		if(strstr(name, name2))
		{
			found = true;
			break;
		}
		
			
		
	}

	//if the sound is already created
	if(found)
	{
		sound->sound->release();
		delete sound;
		return sounds.at(i);
	}

	sounds.push_back(sound);

	return sound;
}

//will not create multiple sounds of same file
Sound* SoundSystem::createSound(const char* filename)
{
	Sound* sound = new Sound();
	result = system->createSound(filename, FMOD_DEFAULT, 0, &sound->sound);
	errorCheck(result);

	char name[255];
	char name2[255];
	bool found = false;
	int i = 0;
	for(i = 0; i < (int)sounds.size(); i++)	//check to se if the sound is already created
	{
		sounds.at(i)->sound->getName(name, 255);
		sound->sound->getName(name2, 255);

		if(strstr(name, name2))
		{
			found = true;
			break;
		}
	}

	//if the sound is already created
	if(found)
	{
		sound->sound->release();
		delete sound;
		return sounds.at(i);
	}

	sounds.push_back(sound);

	return sound;
}

Sound* SoundSystem::createPlaylist(const char* name)
{
	//Ladda in ett sound
	Sound* sound = createSound(name);
	Sound* playlist;
	FMOD_SOUND_TYPE soundtype;

	result = sound->sound->getFormat(&soundtype, 0, 0, 0);
	bool isplaylist = (soundtype == FMOD_SOUND_TYPE_PLAYLIST);

	FMOD_TAG tag;
	int count = 0;

	if(isplaylist)
	{
		playlist = new Playlist();
		((Playlist*)playlist)->count = 0;
		((Playlist*)playlist)->sound = sound->sound;

		//Ladda första låten
		playlist->sound->getTag(0, 0, &tag);
		system->createStream((char*)tag.data, FMOD_DEFAULT, 0, &((Playlist*)playlist)->song);

	}
	else
	{
		return NULL;
	}
	while(playlist->sound->getTag("FILE", count, &tag)==FMOD_OK)
	{
		this->nrOfSounds++;
		count++;
	}
	count=0;
	this->playlist = playlist;

	return playlist;
}

bool SoundSystem::playSound(Sound* s)
{
	result = system->playSound(FMOD_CHANNEL_FREE, s->sound, false, NULL);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::playPlaylist(Sound* playlist)
{
	result = system->playSound(FMOD_CHANNEL_FREE, ((Playlist*)playlist)->song, false, NULL);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::playNextSong(Sound* playlist)
{
	FMOD_TAG tag;
	//((Playlist*)playlist)->count++;

	int tempRand = rand() % this->nrOfSounds;
	while(tempRand==((Playlist*)playlist)->count)
	{
		tempRand = rand() % this->nrOfSounds;
	}
	((Playlist*)playlist)->count = tempRand;

	((Playlist*)playlist)->song->release();

	result = ((Playlist*)playlist)->sound->getTag("FILE", ((Playlist*)playlist)->count, &tag);
	if(result != FMOD_OK)
	{
		((Playlist*)playlist)->count = 0;
		result = ((Playlist*)playlist)->sound->getTag("FILE", ((Playlist*)playlist)->count, &tag);
	}
	
	system->createStream((char*)tag.data, FMOD_DEFAULT, 0, &((Playlist*)playlist)->song);
	system->playSound(FMOD_CHANNEL_FREE, ((Playlist*)playlist)->song, false, &((Playlist*)playlist)->channel);
	cout << ((Playlist*)playlist)->count << endl; 
	return true;
}

bool SoundSystem::isplaying(Sound* playlist)
{
	bool isplaying;
	((Playlist*)playlist)->channel->isPlaying(&isplaying);
	return isplaying;
}

bool SoundSystem::setPaused(Sound* playlist, bool state)
{
	result = ((Playlist*)playlist)->channel->setPaused(state);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::setPaused(bool state)
{
	result = masterChannelGroup->setPaused(state);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::setMute(bool state)
{
	result = masterChannelGroup->setMute(state);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::setVolume(float volume)
{
	result = masterChannelGroup->setVolume(volume);
	if(errorCheck(result))
		return false;

	return true;
}

bool SoundSystem::getPaused()
{
	bool paused;
	result = masterChannelGroup->getPaused(&paused);
	errorCheck(result);

	return paused;
}

bool SoundSystem::getMute()
{
	bool paused;
	result = masterChannelGroup->getMute(&paused);
	errorCheck(result);

	return paused;
}

int SoundSystem::loadPlaylist(string filename)
{
	char input;
	ifstream fin;
	string prefix = "";
//	playlist_testSize = 4;
//	this->playlist_test = new Sound*[playlist_testSize];
//
//	char* soundFile = "";
//
	fin.open(filename);
	if(!fin.is_open())
	{
		return 0;
	}

	while(!fin.eof())
	{
		getline(fin, prefix);
		if(prefix.at(0)!='#')
		{
//			if(nrOfSounds==playlist_testSize)
//			{
//				playlist_testSize += 3;
//				Sound** temp = new Sound*[playlist_testSize];
//				for(int i = 0; i < nrOfSounds; i++)
//				{
//					temp[i] = playlist_test[i];
//					delete playlist_test[i];
//				}
//				delete playlist_test;
//				playlist_test = temp;
//				temp = NULL;
//			}
//
//			soundFile = new char[prefix.size() + 1];
//			strcpy(soundFile, prefix.c_str());
//			playlist_test[nrOfSounds++] = createStream(soundFile);
			this->nrOfSounds++;
		}

	}

//	return playlist_test;
//
}

//int SoundSystem::getNrOfSounds()
//{
//	return this->nrOfSounds;
//}

FMOD_RESULT SoundSystem::getLastError()
{
	return lastError;
}

//returns true if error, false if not.
//prints error message
//sets lastError if there is an error
bool SoundSystem::errorCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		char text[256];
		sprintf_s(text, sizeof(text), "FMOD error! %s", FMOD_ErrorString(result));
		MessageBox(NULL, text, "FMOD error", MB_OK);
		lastError = result;
		return true;
	}

	return false;
}

void SoundSystem::clearLastError()
{
	lastError = FMOD_OK;
}

int SoundSystem::getMemoryUsage()
{
	return memoryUsage;
}

int SoundSystem::getMaxMemoryUsage()
{
	return maxMemoryUsage;
}