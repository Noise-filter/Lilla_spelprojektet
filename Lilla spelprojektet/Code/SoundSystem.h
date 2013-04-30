#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

#pragma comment(lib, "fmodex_vc.lib")

struct Sound
{
	FMOD::Sound* sound;
};

struct Playlist : public Sound
{
	FMOD::Sound* song;
	int count;
	FMOD::Channel* channel;
};

class SoundSystem
{
public:

	static SoundSystem* Getinstance();
	//
	bool init();
	bool shutdown();

	Sound* createStream(const char* filename);
	Sound* createSound(const char* filename);
	Sound* createPlaylist(const char* filename);

	bool playSound(Sound* s);
	bool playPlaylist(Sound* playlist);
	bool playNextSong(Sound* playlist);
	bool isplaying(Sound* playlist);
	bool setPaused(Sound* playlist, bool state);

	bool setPaused(bool state);				// pauses/resumes all sound
	bool setMute(bool state);				// mutes/unmutes all sound

	bool setVolume(float volume);			// 0.0 - 1.0

	bool getPaused();						// get state of master channel
	bool getMute();							// get state of master channel
	
	int loadPlaylist(string filename);
	int getNrOfSounds();
	
	bool update();

	//Error handling
	FMOD_RESULT getLastError();				// return lastError, is FMOD_OK if no error occurred
	bool errorCheck(FMOD_RESULT result);	// checks for error and sets lastError
	void clearLastError();					// sets lastError to FMOD_OK

	int getMemoryUsage();
	int getMaxMemoryUsage();

private:
	FMOD::System* system;
	
	SoundSystem();
	~SoundSystem();
	static SoundSystem* m_SoundSystem;

	std::vector<Sound*> sounds;
	Sound* playlist;
	FMOD::ChannelGroup* masterChannelGroup;

	int nrOfSounds;
	
	FMOD_RESULT result;
	FMOD_RESULT lastError;

	int memoryUsage;
	int maxMemoryUsage;

};


#endif