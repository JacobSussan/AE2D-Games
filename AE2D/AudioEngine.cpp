#include "AudioEngine.h"
#include "AEErrors.h"

#include <iostream>

//-------------------------------------------------------------------
void AE::SoundEffect::play(int loops) 
{
	if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
		if (Mix_PlayChannel(0, m_chunk, loops) == -1) {
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
	}
}

///------------------------------------------------------------------
void AE::Music::play(int loops) 
{
	if (Mix_PlayMusic(m_music, loops)) {
		fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
	}
}

//-------------------------------------------------------------------
void AE::Music::pause() 
{
	Mix_PauseMusic();
}

//-------------------------------------------------------------------
void AE::Music::stop() 
{
	Mix_HaltMusic();
}

//-------------------------------------------------------------------
void AE::Music::resume() 
{
	Mix_ResumeMusic();
}

///------------------------------------------------------------------
AE::AudioEngine::AudioEngine() 
{
	//empty
}

//-------------------------------------------------------------------
AE::AudioEngine::~AudioEngine() 
{
	destroy();
}

//-------------------------------------------------------------------
void AE::AudioEngine::init() 
{
	if (m_isInit) {
		fatalError("Tried to init audio engine twice!");
	}
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
		fatalError("Mix_Init error: " + std::string(Mix_GetError()));
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fatalError("Mix_OpenAudioError error: " + std::string(Mix_GetError()));
	}
	m_isInit = true;
}

//-------------------------------------------------------------------
void AE::AudioEngine::destroy() 
{
	if (m_isInit) {
		m_isInit = false;

		for (auto& it : m_effectMap) {
			Mix_FreeChunk(it.second);
		}
		for (auto& it : m_musicMap) {
			Mix_FreeMusic(it.second);
		}
		m_effectMap.clear();
		m_musicMap.clear();

		Mix_CloseAudio();
		Mix_Quit();
	}
}

//-------------------------------------------------------------------
AE::SoundEffect AE::AudioEngine::loadSoundEffect(const std::string& filePath, float volume) 
{
	auto it = m_effectMap.find(filePath);
	SoundEffect effect;
	if (it == m_effectMap.end()) {
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
		Mix_VolumeChunk(chunk, volume);
		if (chunk == nullptr) {
			fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()) + " | " + filePath);
		}
		effect.m_chunk = chunk;
		m_effectMap[filePath] = chunk;
	} else {
		effect.m_chunk = it->second;
	}
	return effect;
}

//-------------------------------------------------------------------
AE::Music AE::AudioEngine::loadMusic(const std::string& filePath, float volume) 
{
	auto it = m_musicMap.find(filePath);
	Music music;
	if (it == m_musicMap.end()) {
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
		Mix_VolumeMusic(volume);
		if (mixMusic == nullptr) {
			fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()) + " | " + filePath);
		}
		music.m_music = mixMusic;
		m_musicMap[filePath] = mixMusic;
	} else {
		music.m_music = it->second;
	}
	return music;
}


