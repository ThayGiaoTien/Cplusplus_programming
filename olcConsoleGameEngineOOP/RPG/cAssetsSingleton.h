#pragma once
#include"olcConsoleGameEngineOOP.h"
#include<utility>
#include<map>
class cAssetsSingleton
{
// Prevent user to create instace object by making its constructor private
private:
	cAssetsSingleton();
	~cAssetsSingleton();
	
	map<string, olcSprite*> m_mapSprites;


public:
	static cAssetsSingleton& get()
	{
		static cAssetsSingleton me;
		return me;
	}
	// Delete copy constructor
	cAssetsSingleton(cAssetsSingleton const&) = delete;
	// Delete void operator
	void operator= (cAssetsSingleton const&) = delete;
	
	// Call this function once to load all the sprites
	void LoadSprite();
	olcSprite* GetSprite(string name);

	// Call this function once to load all the maps
	void LoadMap();
	wstring GetMap(string name);
};

